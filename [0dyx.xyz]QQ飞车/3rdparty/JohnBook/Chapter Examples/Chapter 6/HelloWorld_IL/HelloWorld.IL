// You need the .assembly for the program to run.
.assembly hello {}

// Declare a "C" like main.
.method static public void main() il managed
{
    // This tells the execution engine (EE) where to start executing.
    // You need one per program.  This directive can apply to methods as
    // well.
    .entrypoint

    // This is not needed for ILASM, but ILDASM will always show it so
    // I included it.
    .maxstack 1

    // Push a string onto the stack.
    ldstr  "Hello World from IL!"

    // Call the System.Console.Writeline class.
    call   void [mscorlib]System.Console::WriteLine(class System.String)

    // Return to the caller.  The file will compile if you forget this,
    // but you will cause an exception in the EE.
    ret
}

