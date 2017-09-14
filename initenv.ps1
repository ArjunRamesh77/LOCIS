# Sets the Build Environment for the LOCIS project

$script_root = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

#--------------------------------------------------------------------------------------------------------------------------------------------------------

[Environment]::SetEnvironmentVariable("OUTPUT_LIBPATH", "$script_root\Commons\lib\", "Process")
[Environment]::SetEnvironmentVariable("OUTPUT_DLLPATH", "$script_root\Commons\dll\", "Process")

#--------------------------------------------------------------------------------------------------------------------------------------------------------

#--------------------------------------------------------[LOCIS PATHS]-----------------------------------------------------------------------------------

# LOCIS root
[Environment]::SetEnvironmentVariable("LOCIS_ROOT", "$script_root\", "Process")

# LOCISFramework root
[Environment]::SetEnvironmentVariable("LOCIS_FW_ROOT", "$script_root\LOCISFrameWork\LOCISFrameWork\", "Process")

# Python home
[Environment]::SetEnvironmentVariable("LOCIS_PYTHON_HOME", "$script_root\..\Python27", "Process")

# Kinsol srcdir 
[Environment]::SetEnvironmentVariable("LOCIS_KINSOL_SRCDIR", "$script_root\Solvers\KINSOL\kinsol_src\srcdir", "Process")

# Kinsol buiddir
[Environment]::SetEnvironmentVariable("LOCIS_KINSOL_BUILDDIR", "$script_root\Solvers\KINSOL\kinsol_src\builddir", "Process")

# Sundials
[Environment]::SetEnvironmentVariable("LOCIS_SUNDIALS", "$script_root\Solvers\KINSOL\kinsol_src\srcdir\src\sundials", "Process")

# Ida srcdir
[Environment]::SetEnvironmentVariable("LOCIS_IDA_SRCDIR", "$script_root\Solvers\IDA\IDA_src\srcdir", "Process")

# Ida builddir
[Environment]::SetEnvironmentVariable("LOCIS_IDA_BUILDDIR", "$script_root\Solvers\IDA\IDA_src\builddir", "Process")



& devenv