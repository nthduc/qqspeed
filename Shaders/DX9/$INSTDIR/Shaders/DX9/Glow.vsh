//
// Generated by Microsoft (R) D3DX9 Shader Compiler 9.12.589.0000
//
//   fxc /T vs_1_1 /E VSGlow /Fc a.vsh a.fx
//
//
// Parameters:
//
//   float4 GlowAmbient;
//   float4 GlowColor;
//   float GlowThickness;
//   float4x4 Projection;
//   float4x3 WorldView;
//
//
// Registers:
//
//   Name          Reg   Size
//   ------------- ----- ----
//   Projection    c0       4
//   WorldView     c4       3
//   GlowColor     c7       1
//   GlowAmbient   c8       1
//   GlowThickness c9       1
//
//
// Default values:
//
//   GlowColor
//     c7   = { 0.5, 0.2, 0.2, 1 };
//
//   GlowAmbient
//     c8   = { 0.2, 0.2, 0, 0 };
//
//   GlowThickness
//     c9   = { 0.015, 0, 0, 0 };
//

    vs_1_1
    def c10, -1, 1, 0, 0
    dcl_position v0
    dcl_normal v1
    dp3 r0.x, v1, c4
    dp3 r0.y, v1, c5
    dp3 r0.z, v1, c6
    dp3 r1.x, r0, r0
    rsq r0.w, r1.x
    mul r1.xyz, r0, r0.w
    dp4 r0.x, v0, c4
    dp4 r0.y, v0, c5
    dp4 r0.z, v0, c6
    mad r0.xyz, c9.x, r1, r0
    mad r1.w, r1.z, r1.z, c10.x
    mov r0.w, c10.y
    dp4 oPos.x, r0, c0
    dp4 oPos.y, r0, c1
    dp4 oPos.z, r0, c2
    mul r1.w, r1.w, r1.w
    dp4 oPos.w, r0, c3
    mov r0, c7
    mad oD0, r0, r1.w, c8

// approximately 19 instruction slots used