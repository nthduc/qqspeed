// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net

#include <NiMain.h>
#include <NiAnimation.h>
#include <NiCollision.h>
#include "NiQuadTreeGroundPartition.h"

char* gpcInFile = NULL;
char* gpcOutFile = NULL;
char* gpcTriangles = NULL;
int giMaxTrianglePerPartition = 30;
bool gbDebug = false;
bool gbShareTriangles = false;



//---------------------------------------------------------------------------
void PrintUsage()
{
    fprintf(stderr, 
        "Usage: meshdice -in input_filename -out output_filename\n");
}
//---------------------------------------------------------------------------
bool CheckArguments(int argc, char** argv)
{
    //
    // Command line argument checking 
    // 
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            char* pc = argv[i];
            while (*pc)
            {
                *pc = tolower(*pc); 
                pc++;
            }
        }

        if (!strcmp("-in", argv[i]))
        {
            if ((i + 1) >= argc)
                return false;

            gpcInFile = NiAlloc(char, strlen(argv[i + 1]) + 1);
            if (gpcInFile == NULL)
                return false;
            strcpy(gpcInFile, argv[i + 1]);
            i++;
        }

        if (!strcmp("-out", argv[i]))
        {
            if ((i + 1) >= argc)
                return false;

            gpcOutFile = NiAlloc(char, strlen(argv[i + 1]) + 1);
            if (gpcOutFile == NULL)
                return false;
            strcpy(gpcOutFile, argv[i + 1]);
            i++;
        }

        if (!strcmp("-t", argv[i]))
        {
            if ((i + 1) >= argc)
                return false;

            gpcTriangles = NiAlloc(char, strlen(argv[i + 1]) + 1);
            if (gpcTriangles == NULL)
                return false;
            strcpy(gpcTriangles, argv[i + 1]);

            giMaxTrianglePerPartition = atoi(gpcTriangles);
            if (giMaxTrianglePerPartition <= 0)
                giMaxTrianglePerPartition = 28;

            i++;
        }

        // Enable triangle sharing for quad trees
        if (!strcmp("-st", argv[i]))
        {
            gbShareTriangles = true;
        }

        if (!strcmp("-d", argv[i]))
        {
            gbDebug = true;
        }
    }

    if (!gpcInFile || !gpcOutFile)
    {
        PrintUsage();
        return false;
    }
    
    return true;
}
//---------------------------------------------------------------------------
void Cleanup()
{
    NiFree(gpcInFile);
    NiFree(gpcOutFile);
    NiFree(gpcTriangles);
}
//---------------------------------------------------------------------------
void RemoveEmptyShapesAndAllNormals(NiAVObject* pkObject)
{
    if (NiIsKindOf(NiTriBasedGeom, pkObject))
    {
        NiTriBasedGeom* pkGeom = (NiTriBasedGeom*)pkObject;

        if (pkGeom->GetVertexCount() == 0)
        {
            assert(pkGeom->GetParent());
            pkGeom->GetParent()->DetachChild(pkGeom);
        }
        else
        {
            NiGeometryData* pkData = pkGeom->GetModelData();
            pkData->Replace(pkData->GetVertexCount(), pkData->GetVertices(),
                0, 0, 0, 0, NiGeometryData::NBT_METHOD_NONE);
        }
    }

    if (NiIsKindOf(NiNode, pkObject))
    {
        NiNode* pkNode = (NiNode*)pkObject;

        for (unsigned int i = 0; i < pkNode->GetArrayCount(); i++)
        {
            NiAVObject* pkChild = pkNode->GetAt(i);

            if (pkChild)
                RemoveEmptyShapesAndAllNormals(pkChild);
        }
    }
}
//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
    NiInit();

    if (!CheckArguments(argc, argv))
    {
        Cleanup();

        return 1;
    }

    //
    // Load nif file
    //
    NiStream* pkStream = NiNew NiStream();

    fprintf(stdout, "Loading scene from %s\n", gpcInFile);

    if (!pkStream->Load(gpcInFile))
    {
        fprintf(stderr, "Error loading stream.\n");
        Cleanup();

        return 1;
    }

    int iCount = pkStream->GetObjectCount();
    
    if (iCount < 1) 
    {
        fprintf(stderr, "No top-level file objects.\n");
        Cleanup();

        return 1;
    }
    
    NiNodePtr spNode;

    for (int i = 0; i < iCount; i++)
    {
        NiObject* pkObject = pkStream->GetObjectAt(i);

        if (NiIsKindOf(NiNode, pkObject))
        {
            spNode = (NiNode*)pkObject;
            break;
        }
    }

    if (!spNode)
    {
        fprintf(stderr, "No top-level node objects.\n");
        Cleanup();

        return 1;
    }

    spNode->Update(0.0f);
    NiCollisionTraversals::CreateWorldVertices(spNode);
    NiCollisionTraversals::UpdateWorldVertices(spNode);
    NiCollisionTraversals::CreateWorldNormals(spNode);
    NiCollisionTraversals::UpdateWorldNormals(spNode);
    
    // Assumptions:
    // Assumes positive Z is up
    // Assumes the ground will never move
    // Assumes object has world vertices AND world normals
    NiQuadTreeGroundPartition* pkGround = (NiQuadTreeGroundPartition*) 
        NiNew NiQuadTreeGroundPartition(giMaxTrianglePerPartition,
            gbShareTriangles); 

    pkGround->BuildGround(spNode);

    NiAVObjectPtr spDiced = pkGround->GetGround();
    NiDelete pkGround;

    // gbDebug will let you see the parititioning. For game purposses you will
    // want to remove normals and vertex colors to save space.
    if (!gbDebug)
    {
        RemoveEmptyShapesAndAllNormals(spDiced);
    }

    fprintf(stdout, "Saving scene to %s\n", gpcOutFile);

    pkStream->RemoveAllObjects();
    pkStream->InsertObject(spDiced);

    if (!pkStream->Save(gpcOutFile))
    {
        fprintf(stderr, "Error saving stream.\n");
        Cleanup();

        return 1;
    }

    pkStream->RemoveAllObjects();

    spNode = 0;
    spDiced =0;

    NiDelete pkStream;

    Cleanup();
    NiShutdown();

    return 0;
}
//---------------------------------------------------------------------------
