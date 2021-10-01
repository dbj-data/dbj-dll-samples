
## Is this still true in 2021?

By default calling convention of functions in C# is `__stdcall` but in C/C++ default is `__cdecl`, so you should change calling convention of your function as follow:

```cs
[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void TimesTree( uint frame );

[DllImport("SimpleDLL.dll")]
public static extern System.UInt32 DllFunctionPointer( 
uint i, 

[MarshalAs(UnmanagedType.FunctionPtr)] TimesTree callback 

) ;

static unsafe void Main(string[] args)
{
    // ...
    System.UInt32 jRet = DllFunctionPointer(j, CallBack );
    // ...
}
```