# Script to copyu necessary dll into the projects configuration directory

$script_root = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Copy all generic libs
$dbg_lib_path = "$script_root\Commons\lib\Debug"
$rel_lib_path = "$script_root\Commons\lib\Release"


# Project configuration paths
$kinsol_release = "$script_root\Solvers\KINSOL\Release"
$kinsol_debug = "$script_root\Solvers\KINSOL\Debug"
$ida_release = "$script_root\Solvers\IDA\Release"
$ida_debug = "$script_root\Solvers\IDA\Debug"

# Get Arguments
$project_name = $args[0]
if($project_name -ne $null)
{
    # Copy specific adolc configuration for KINSOL/IDA
    if($project_name.ToLower() -eq "kinsol" -or $project_name.ToLower() -eq "ida")
    {
        # ADOL-C
        $adolc_conf = $args[1]
        if($adolc_conf -eq $null)
        {
            return
        }

        if($adolc_conf.ToLower() -eq "sparse" -or $adolc_conf.ToLower() -eq "nosparse")
        {
            $proj_conf = $args[2]
            if($proj_conf -eq $null)
            {
                return
            }

            # Copy dlls    
            if(test-path "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\$adolc_conf\adolc.dll")
            {
                copy "$script_root\Commons\dll\ADOL-C\$adolc_conf\adolc.dll" "$script_root\Solvers\$project_name\$proj_conf"
                write-host ":)"
            }
            else
            {
                write-host ":(" -foregroundcolor "red"
            }
        }
        return
    }
}

#///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
# KLU(lib)
if(test-path "$script_root\Solvers\SUITESPARSE\builddir\lib\Debug\libklud.lib")
{
     copy "$script_root\Solvers\SUITESPARSE\builddir\lib\Debug\libklud.lib" $dbg_lib_path
     pushd $dbg_lib_path
     if(test-path libklu.lib)
     {
        del libklu.lib
     }
     ren libklud.lib libklu.lib
     popd
     write-host "libklu.lib debug :)"
}
else
{
    write-host "libklu.lib debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\SUITESPARSE\builddir\lib\Release\libklu.lib")
{
     copy "$script_root\Solvers\SUITESPARSE\builddir\lib\Release\libklu.lib" $rel_lib_path
     write-host "libklu.lib release :)"
}
else
{
    write-host "libklu.lib release :(" -foregroundcolor "red"
}

#///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ADOL-C(lib+dll)
# Copy libs   
if(test-path "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\sparse\adolc.lib")
{
    copy "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\sparse\adolc.lib" "$script_root\Commons\lib\ADOL-C\sparse"
    write-host "adolc.lib sparse :)"
}
else
{
    write-host "adolc.lib sparse :(" -foregroundcolor "red"
}
 
if(test-path "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\nosparse\adolc.lib")
{
    copy "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\nosparse\adolc.lib" "$script_root\Commons\lib\ADOL-C\nosparse"
    write-host "adolc.lib nosparse :)"
}
else
{
    write-host "adolc.lib nosparse :(" -foregroundcolor "red"
}

# Copy dlls  
if(test-path "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\sparse\adolc.dll")
{
    copy "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\sparse\adolc.dll" "$script_root\Commons\dll\ADOL-C\sparse"
    write-host "adolc.dll sparse :)"
} 
else
{
    write-host "adolc.dll sparse :(" -foregroundcolor "red"
}
 
if(test-path "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\nosparse\adolc.dll")
{
    copy "$script_root\Solvers\ADOL-C\MSVisualStudio\v14\nosparse\adolc.dll" "$script_root\Commons\dll\ADOL-C\nosparse"
    write-host "adolc.dll nosparse :)"
}
else
{
    write-host "adolc.dll nosparse :(" -foregroundcolor "red"
}

#///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
# LAPACK (dll+lib)
if(test-path "$script_root\Solvers\LAPACK\builddir\lib\libblas.lib")
{
    copy "$script_root\Solvers\LAPACK\builddir\lib\libblas.lib" "$script_root\Commons\lib\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\lib\libblas.lib" "$script_root\Commons\lib\Debug\"
    write-host "libblas.lib Release|Debug :)"
}
else
{
    write-host "libblas.lib Release|Debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\LAPACK\builddir\lib\liblapack.lib")
{
    copy "$script_root\Solvers\LAPACK\builddir\lib\liblapack.lib" "$script_root\Commons\lib\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\lib\liblapack.lib" "$script_root\Commons\lib\Debug\"
    write-host "liblapack.lib Release|Debug :)"
}
else
{
    write-host "liblapack.lib Release|Debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\LAPACK\builddir\lib\libtmglib.lib")
{
    copy "$script_root\Solvers\LAPACK\builddir\lib\libtmglib.lib" "$script_root\Commons\lib\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\lib\libtmglib.lib" "$script_root\Commons\lib\Debug\"
    write-host "libtmglib.lib Release|Debug :)"
}
else
{
    write-host "libtmglib.lib Release|Debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll")
{
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" "$script_root\Commons\dll\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" "$script_root\Commons\dll\Debug\"

    # Copy to Project's directory
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" $kinsol_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" $kinsol_debug
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" $ida_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\libblas.dll" $ida_debug

    write-host "llibblas.dll Release|Debug :)"
}
else
{
    write-host "libblas.dll Release|Debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll")
{
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" "$script_root\Commons\dll\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" "$script_root\Commons\dll\Debug\"

    # Copy to Project's directory
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" $kinsol_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" $kinsol_debug
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" $ida_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\liblapack.dll" $ida_debug

    write-host "liblapack.dll Release|Debug :)" 
}
else
{
    write-host "liblapack.dll Release|Debug :(" -foregroundcolor "red"
}

if(test-path "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll")
{
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" "$script_root\Commons\dll\Release\"
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" "$script_root\Commons\dll\Debug\"

    # Copy to Project's directory
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" $kinsol_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" $kinsol_debug
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" $ida_release
    copy "$script_root\Solvers\LAPACK\builddir\bin\libtmglib.dll" $ida_debug

    write-host "libtmglib.dll Release|Debug :)"
}
else
{
    write-host "libtmglib.dll Release|Debug :(" -foregroundcolor "red"
}

#///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
# MinGW Runtime (dll)
if(test-path "C:\MinGW\bin\libgfortran-3.dll")
{

    # Copy to Project's directory
    copy "C:\MinGW\bin\libgfortran-3.dll" $kinsol_release
    copy "C:\MinGW\bin\libgfortran-3.dll" $kinsol_debug
    copy "C:\MinGW\bin\libgfortran-3.dll" $ida_release
    copy "C:\MinGW\bin\libgfortran-3.dll" $ida_debug

    write-host "libgfortran-3.dll(MinGW) Release|Debug :)"
}
else
{
    write-host "libgfortran-3.dll(MinGW) Release|Debug :(" -foregroundcolor "red"
}

if(test-path "C:\MinGW\bin\libgcc_s_dw2-1.dll")
{

    # Copy to Project's directory
    copy "C:\MinGW\bin\libgcc_s_dw2-1.dll" $kinsol_release
    copy "C:\MinGW\bin\libgcc_s_dw2-1.dll" $kinsol_debug
    copy "C:\MinGW\bin\libgcc_s_dw2-1.dll" $ida_release
    copy "C:\MinGW\bin\libgcc_s_dw2-1.dll" $ida_debug

    write-host "libgcc_s_dw2-1.dll(MinGW) Release|Debug :)"
}
else
{
    write-host "libgcc_s_dw2-1.dll(MinGW) Release|Debug :(" -foregroundcolor "red"
}


