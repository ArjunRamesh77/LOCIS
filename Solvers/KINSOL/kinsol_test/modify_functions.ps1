# This script creates a modiefied functions.cpp file for kinsol_test project
$num_max_threads = 100

$script_root = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Modifying files
$functions_cpp =  "$script_root\functions.cpp"
$local_func_prototypes_h =  "$script_root\local_func_prototypes.h"
$thread_data_h =  "$script_root\thread_data.h"
$extra_headers_h =  "$script_root\extra_headers.h"

$test_files = Get-Childitem -Path "$script_root\test_cases" -Name -include "*.kint"
$tfout_path = "$script_root\test_output"


# Main loop area
for($n = 0; $n -lt $test_files.Count; $n++)
{

    $tfile_name = $test_files[$n]
    $test_file = Get-Content "$script_root\test_cases\$tfile_name"
    write-host "BUILDING CASE = $tfile_name"
        
    $output = "$tfout_path\$tfile_name"

    #Write header section
    write-output ' ' | out-file $functions_cpp -encoding ascii 
    write-output ' ' | out-file $local_func_prototypes_h -encoding ascii 
    write-output "" | out-file $thread_data_h -encoding ascii 
    write-output "" | out-file $extra_headers_h -encoding ascii 

#---------------------------------------------------------------------- MODIFY FILE SECTION -------------------------------------------------------------------------------

    write-output '#include <iostream>' | out-file $functions_cpp -encoding ascii -append
    write-output '#include <fstream>' | out-file $functions_cpp -encoding ascii -append
    write-output '#include "kinsol_helpers.h"' | out-file $functions_cpp -encoding ascii -append
    write-output '#include "adolc\adolc.h"' | out-file $functions_cpp -encoding ascii -append
    write-output '#include "local_func_prototypes.h"' | out-file $functions_cpp -encoding ascii -append
    write-output '#include <thread>' | out-file $functions_cpp -encoding ascii -append
    write-output '#include <mutex>' | out-file $functions_cpp -encoding ascii -append
    #include extern file for thread data
    write-output "#include `"thread_data.h`"" | out-file $functions_cpp -encoding ascii -append
    write-output "#include `"extra_headers.h`"" | out-file $functions_cpp -encoding ascii -append
    write-output ' ' | out-file $functions_cpp -encoding ascii -append
    
    write-output 'using namespace std;' | out-file $functions_cpp -encoding ascii -append

    #Global variables
    write-output 'std::mutex mtx1;' | out-file $functions_cpp -encoding ascii -append
    write-output "std::thread::id THID[$num_max_threads];" | out-file $functions_cpp -encoding ascii -append

    #Thread safe Jacobian Function
    write-output 'void JacFunc(double *x, double **J);' | out-file $functions_cpp -encoding ascii -append

    #Output function
    write-output 'void WriteOutput(std::string fname);' | out-file $functions_cpp -encoding ascii -append

    # Main loop to read template
    $got_thread = 0
    $got_function = 0
    $nt = 0
    $thread_names  = new-object 'System.Collections.Generic.List[string]'
    for($i = 0; $test_file[$i] -ne $null; $i++)
    {
        $line = $test_file[$i]
        $line = $line.TrimEnd()
        $line_blank = $line.Trim()
        
        # Skip blank lines
        if($line_blank -eq "")
        {
            continue
        }
        
        # Skip Comments
        $matches = ([regex]"^[ \t]*#(.*)").Matches($line)
        if($matches[0].value -ne $null)
        {
            continue        
        }
        
        # Get THREAD
        $matches = ([regex]"^[ \t]*THREAD[ \t]+(.*)").Matches($line)
        if($matches[0].value -ne $null)
        {
            # Get Thread Name
            $thread_name = $matches[0].Groups[1].value
            $thread_names.Add($thread_name.Trim())
            
            $nt++
            write-output "//------------------------------------------ THREAD $($nt) ------------------------------------------" | out-file $functions_cpp -encoding ascii -append
            write-output "//------------------------------------------ THREAD $($nt) ------------------------------------------" | out-file $local_func_prototypes_h -encoding ascii -append
            write-output "std::vector<std::string> THREAD_OUTPUT$nt;" | out-file $thread_data_h -encoding ascii -append
            $got_thread = 1
            continue        
        }
        
        # Get function
        if($got_thread -eq 1)
        {
            $matches = ([regex]"^[ \t]*ROUTINE[ \t]+(.*)").Matches($line)
            if($matches[0].value -ne $null)
            {
                $func_name = $matches[0].Groups[1].Value
                $got_function = 1
                $func_data  = new-object 'System.Collections.Generic.List[string]'
                continue        
            }
        }
        
        # Add function data
        if($got_function -eq 1)
        {        
            # Look for function end
            $matches = ([regex]"^[ \t]*END").Matches($line)
            if($matches[0].value -ne $null)
            {
                # End of function
                $inter_name = $func_name.ToUpper().Trim()
                
                # Switch between different functions
                if($inter_name -eq "SETSIZE")
                {
                    $func_header = "void SetSize_$nt(int &NVAR, int &NEQ) {"
                }
                elseif($inter_name -eq "SETGUESS")
                {
                   $func_header = "void SetGuess_$nt(int NVAR, double* guess) {"
                }
                elseif($inter_name -eq "SETOPTIONS")
                {
                    $func_header = "void SetOptions_$nt(KINSOL_HELPERS::KinsolOptions *options) {"   
                }
                elseif($inter_name -eq "RESFUNC")
                {
                    $func_header = "void ResFunc_$nt(double *x, double *r) {"    
                }
                elseif($inter_name -eq "JACFUNC")
                {
                    $func_header = "void JacFunc_$nt(double *x, double **J) {"
                }
                elseif($inter_name -eq "INFO")
                {
                    $func_header = "void UserInfoHandler_$nt(void *s, int type) {" 
                    
                    # basic 
                	$func_data.Add("     if (type == 0) {")
                	$func_data.Add("	     KINSOL_HELPERS::infomsgs* info = (KINSOL_HELPERS::infomsgs*)s;")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> KINSOL INFO `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     function :: `" + info->function);")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     module :: `" + info->module);")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     msg :: `" + info->msg);")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }");  
                    $func_data.Add("     else if (type == 1)")
                	$func_data.Add("     {");
                	$func_data.Add("	     KINSOL_HELPERS::finalstatsl1* fs1 = (KINSOL_HELPERS::finalstatsl1*)s;");
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> KINSOL FINAL STATISTICS 1 `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumNonlinSolvIters :: `" + to_string(fs1->nni));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumFuncEvals :: `" + to_string(fs1->nfe));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetNumJacEvals :: `" + to_string(fs1->nje));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetNumFuncEvals :: `" + to_string(fs1->nfeD));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }")
                	$func_data.Add("     else if (type == 2)")
                	$func_data.Add("     {")
                	$func_data.Add("	     KINSOL_HELPERS::finalstatsl2* fs2 = (KINSOL_HELPERS::finalstatsl2*)s;")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> KINSOL FINAL STATISTICS 2 `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumNonlinSolvIters :: `" + to_string(fs2->nni));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumFuncEvals :: `" + to_string(fs2->nfe));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetNumJacEvals :: `" + to_string(fs2->nje));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetNumFuncEvals :: `" + to_string(fs2->nfeD));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetWorkSpace(1) :: `" + to_string(fs2->leniw));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetWorkSpace(2) :: `" + to_string(fs2->lenrw));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumBetaCondFails :: `" + to_string(fs2->nbcfails));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetNumBacktrackOps :: `" + to_string(fs2->nbacktr));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetFuncNorm :: `" + to_string(fs2->fnorm));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINGetStepLength :: `" + to_string(fs2->steplength));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetWorkSpace(1) :: `" + to_string(fs2->leniwB));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetWorkSpace(2) :: `" + to_string(fs2->lenrwB));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     KINDlsGetLastFlag :: `" + to_string(fs2->lsflag));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }")
                      
                }
                elseif($inter_name -eq "ERR")
                {
                    $func_header = "int UserErrorHandler_$nt(void *s, int type) {"
                    
                    # basic
                    $func_data.Add("     if (type == 0 || type == 1 || type == 2) {")
                	$func_data.Add("          KINSOL_HELPERS::error* erro = (KINSOL_HELPERS::error*)s;")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"     >> KINSOL ERROR `");")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"     code :: `" + to_string(erro->ErrCode));")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"     type :: `" + erro->ErrType);")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"     msg :: `");")
                	$func_data.Add("          for (vector<string>::iterator it = erro->ErrLines.begin(); it != erro->ErrLines.end(); it++)")
                	$func_data.Add("          {");
                	$func_data.Add("               THREAD_OUTPUT$nt.push_back(`"          `" + *it);")
                	$func_data.Add("          }")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"  `");")
            	    $func_data.Add("     }");
                    $func_data.Add("     return 0;");
                }
                elseif($inter_name -eq "HEADERS")
                {
                    for($j = 0; $func_data[$j] -ne $null; $j++)
                    {
                        write-output $func_data[$j] | out-file $extra_headers_h -encoding ascii -append    
                    }
                    continue
                }
                
                write-output $func_header | out-file $functions_cpp -encoding ascii -append
                $for_header = $func_header -Replace '[ \t]*{',';'
                write-output $for_header | out-file $local_func_prototypes_h -encoding ascii -append
                for($j = 0; $func_data[$j] -ne $null; $j++)
                {
                    write-output $func_data[$j] | out-file $functions_cpp -encoding ascii -append    
                }
                 #Write Options in output
                if($inter_name -eq "SETOPTIONS")
                {
                    write-output "     THREAD_OUTPUT$nt.push_back(`" `");" | out-file $functions_cpp -encoding ascii -append  
                    write-output "     THREAD_OUTPUT$nt.push_back(`">> OPTIONS FOR SYSTEM `");" | out-file $functions_cpp -encoding ascii -append  
                    for($j = 0; $func_data[$j] -ne $null; $j++)
                    {
                        $func_data[$j] = $func_data[$j] -replace "`""," "
                        $func_data[$j] = $func_data[$j] -replace ";"," "
                        write-output "     THREAD_OUTPUT$nt.push_back(`"$($func_data[$j])`");" | out-file $functions_cpp -encoding ascii -append    
                    }
                }
                write-output "}" | out-file $functions_cpp -encoding ascii -append
                write-output " " | out-file $functions_cpp -encoding ascii -append
                
                $got_function = 0  
                continue
            }
            $func_data.Add($line)
            continue
        }
        
        # Look for Thread end
        if($got_function -eq 0 -and $got_thread -eq 1)
        {
            $matches = ([regex]"^[ \t]*END").Matches($line)
            if($matches[0].value -ne $null)
            {
                # Write
                write-output "void RunKinsol_$nt(KINSOL_HELPERS::f_Init funcinit, KINSOL_HELPERS::f_setdata funcsetdata, KINSOL_HELPERS::f_setoptions funcsetoptions, KINSOL_HELPERS::f_funcisolve funcsolve, KINSOL_HELPERS::f_free funcfree) {" | out-file $functions_cpp -encoding ascii -append
                write-output "void RunKinsol_$nt(KINSOL_HELPERS::f_Init funcinit, KINSOL_HELPERS::f_setdata funcsetdata, KINSOL_HELPERS::f_setoptions funcsetoptions, KINSOL_HELPERS::f_funcisolve funcsolve, KINSOL_HELPERS::f_free funcfree);" | out-file $local_func_prototypes_h -encoding ascii -append
                write-output "     int objnum;" | out-file $functions_cpp -encoding ascii -append
                write-output "     int NVAR, NEQ;" | out-file $functions_cpp -encoding ascii -append
                write-output "     THID[$($nt - 1)] = std::this_thread::get_id();" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     KINSOL_HELPERS::error_kinsol* errorvector = new KINSOL_HELPERS::error_kinsol;" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     SetSize_$nt(NVAR, NEQ);" | out-file $functions_cpp -encoding ascii -append
                write-output "     double* guess = new double[NVAR];" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     SetGuess_$nt(NVAR, guess);" | out-file $functions_cpp -encoding ascii -append
                
                #Write Guess to global vector
                write-output "     THREAD_OUTPUT$nt.push_back(`" `");" | out-file $functions_cpp -encoding ascii -append
                write-output "     THREAD_OUTPUT$nt.push_back(`">> GUESS FOR SYSTEM`");" | out-file $functions_cpp -encoding ascii -append
                write-output "     for (int i = 0; i < NVAR; i++)" | out-file $functions_cpp -encoding ascii -append
                write-output "          THREAD_OUTPUT$nt.push_back(`"     x[`" + std::to_string(i) + `"] = `" + std::to_string(guess[i]));" | out-file $functions_cpp -encoding ascii -append
                
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     KINSOL_HELPERS::KinsolOptions* options = new KINSOL_HELPERS::KinsolOptions;" | out-file $functions_cpp -encoding ascii -append
                write-output "     SetOptions_$nt(options);" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     double* ans = new double[NVAR];" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     objnum =            funcinit(0);"                                                                                                         | out-file $functions_cpp -encoding ascii -append
                write-output "     if(objnum < 0) {"                                                                                                                       | out-file $functions_cpp -encoding ascii -append
                write-output "          KINSOL_HELPERS::error* erro = new KINSOL_HELPERS::error; "                                                                           | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                 | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_INIT returned with a fatal error, program will stop`"); "                                             | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return; }"                                                                               | out-file $functions_cpp -encoding ascii -append

                write-output "     int setdata_rc =    funcsetdata(objnum, errorvector, NVAR, NEQ, guess, ResFunc_$nt, JacFunc, UserInfoHandler_$nt, UserErrorHandler_$nt);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(setdata_rc < 0) {"                                                                                                                   | out-file $functions_cpp -encoding ascii -append
                write-output "          KINSOL_HELPERS::error* erro = new KINSOL_HELPERS::error; "                                                                           | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                 | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SETDATA returned with a fatal error, program will stop`"); "                                          | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                | out-file $functions_cpp -encoding ascii -append

                write-output "     int setoptions_rc = funcsetoptions(objnum, options);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(setoptions_rc <= 0) {"                                                                                                                | out-file $functions_cpp -encoding ascii -append
                write-output "          KINSOL_HELPERS::error* erro = new KINSOL_HELPERS::error; "                                                                           | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                 | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SETOPTIONS returned with a fatal error, program will stop`"); "                                       | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                | out-file $functions_cpp -encoding ascii -append

                write-output "     int solve_rc =      funcsolve(objnum, ans);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(solve_rc <= 0) {"                                                                                                                     | out-file $functions_cpp -encoding ascii -append
                write-output "          KINSOL_HELPERS::error* erro = new KINSOL_HELPERS::error; "                                                                           | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                 | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SOLVE returned with a fatal error, program will stop`"); "                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                | out-file $functions_cpp -encoding ascii -append

                write-output "     funcfree(objnum);" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                
                #Write Final Solution to global vector
                write-output "     THREAD_OUTPUT$nt.push_back(`" `");" | out-file $functions_cpp -encoding ascii -append
                write-output "     THREAD_OUTPUT$nt.push_back(`">> SOLUTION FOR SYSTEM`");" | out-file $functions_cpp -encoding ascii -append
                write-output "     for (int i = 0; i < NVAR; i++)" | out-file $functions_cpp -encoding ascii -append
                write-output "          THREAD_OUTPUT$nt.push_back(`"     x[`" + std::to_string(i) + `"] = `" + std::to_string(ans[i]));" | out-file $functions_cpp -encoding ascii -append
     
                write-output "     delete options;" | out-file $functions_cpp -encoding ascii -append
                write-output "     delete errorvector;" | out-file $functions_cpp -encoding ascii -append
                write-output "}" | out-file $functions_cpp -encoding ascii -append
                $got_thread = 0
            }
        }
    }

    # Write Run Section
    write-output "//------------------------------------------ RUN ;)------------------------------------------" | out-file $functions_cpp -encoding ascii -append
    write-output "void RunTest(KINSOL_HELPERS::f_Init funcinit, KINSOL_HELPERS::f_setdata funcsetdata, KINSOL_HELPERS::f_setoptions funcsetoptions, KINSOL_HELPERS::f_funcisolve funcsolve, KINSOL_HELPERS::f_free funcfree) {" | out-file $functions_cpp -encoding ascii -append
    write-output "" | out-file $functions_cpp -encoding ascii -append

        write-output "     //PARALLEL TEST" | out-file $functions_cpp -encoding ascii -append
        # Write Thread spawns ( Parallel section)
        for($i = 1; $i -le $nt; $i++)
        {
            write-output "     std::thread THREAD$i(RunKinsol_$i, funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);" | out-file $functions_cpp -encoding ascii -append

        }
        write-output "" | out-file $functions_cpp -encoding ascii -append

        # Write thread joins
        for($i = 1; $i -le $nt; $i++)
        {
            write-output "     THREAD$i.join();" | out-file $functions_cpp -encoding ascii -append
        }
        $file_name_parallel = "$($output).par" -replace "\\","\\"
        write-output "     WriteOutput(`"$file_name_parallel`");" | out-file $functions_cpp -encoding ascii -append
        $file_name_parallel_sep = "$script_root\test_output\parallel\$($test_files[$n]).out" -replace "\\","\\"
        write-output "     WriteOutput(`"$file_name_parallel_sep`");" | out-file $functions_cpp -encoding ascii -append
        
        # Clear Thread Output data
        for($i = 1; $i -le $nt; $i++)
        {
            write-output "     THREAD_OUTPUT$i.clear();" | out-file $functions_cpp -encoding ascii -append
        }
        
        write-output " " | out-file $functions_cpp -encoding ascii -append
        write-output "     //SERIAL TEST" | out-file $functions_cpp -encoding ascii -append
        # Write Thread spawns ( Serial section)
        for($i = 1; $i -le $nt; $i++)
        {
            $j = $i + $nt
            write-output "     std::thread THREAD$j(RunKinsol_$i, funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);" | out-file $functions_cpp -encoding ascii -append
            write-output "     THREAD$j.join();" | out-file $functions_cpp -encoding ascii -append

        }
        write-output "" | out-file $functions_cpp -encoding ascii -append
        $file_name_serial = "$($output).ser" -replace "\\","\\"
        write-output "     WriteOutput(`"$file_name_serial`");" | out-file $functions_cpp -encoding ascii -append
        $file_name_serial_sep = "$script_root\test_output\serial\$($test_files[$n]).out" -replace "\\","\\"
        write-output "     WriteOutput(`"$file_name_serial_sep`");" | out-file $functions_cpp -encoding ascii -append
        
        # Clear Thread Output data
        for($i = 1; $i -le $nt; $i++)
        {
            write-output "     THREAD_OUTPUT$i.clear();" | out-file $functions_cpp -encoding ascii -append
        }
        
    write-output "}" | out-file $functions_cpp -encoding ascii -append
    write-output " " | out-file $functions_cpp -encoding ascii -append

    # Write Thread safe Jacobian
    write-output "void JacFunc(double *x, double **J) {" | out-file $functions_cpp -encoding ascii -append
    write-output "     mtx1.lock();" | out-file $functions_cpp -encoding ascii -append

    for($i = 0; $i -lt $nt; $i++)
    {
        if($i -gt 0)
        {
            write-output "     else if (std::this_thread::get_id() == THID[$i]) {" | out-file $functions_cpp -encoding ascii -append  
        }
        else
        {
            write-output "     if (std::this_thread::get_id() == THID[$i]) {" | out-file $functions_cpp -encoding ascii -append
        }
        
        write-output "          JacFunc_$($i + 1)(x, J);" | out-file $functions_cpp -encoding ascii -append       
        write-output "     }" | out-file $functions_cpp -encoding ascii -append    
    }

    write-output "     mtx1.unlock();" | out-file $functions_cpp -encoding ascii -append
    write-output "}" | out-file $functions_cpp -encoding ascii -append
    write-output " " | out-file $functions_cpp -encoding ascii -append

    # Write Output function
    write-output "void WriteOutput(std::string fname) {" | out-file $functions_cpp -encoding ascii -append
    write-output "     std::ofstream out;" | out-file $functions_cpp -encoding ascii -append
    write-output "     out.open(fname);" | out-file $functions_cpp -encoding ascii -append
    write-output " " | out-file $functions_cpp -encoding ascii -append

    for($i = 1; $i -le $nt; $i++)
    {
        write-output "     out <<`"------------------------------THEAD OUTPUT = $($thread_names[$($i - 1)])----------------------------------------------------------------------------------------`";" | out-file $functions_cpp -encoding ascii -append
        write-output "     for (std::vector<string>::iterator it = THREAD_OUTPUT$i.begin(); it != THREAD_OUTPUT$i.end(); it++) {" | out-file $functions_cpp -encoding ascii -append
        write-output "          out << *it<<`"\n`";" | out-file $functions_cpp -encoding ascii -append
        write-output "     }" | out-file $functions_cpp -encoding ascii -append
        write-output " " | out-file $functions_cpp -encoding ascii -append
    }
    write-output "}" | out-file $functions_cpp -encoding ascii -append
    
#---------------------------------------------------------------------- BUILD AND RUN PROJECT ------------------------------------------------------------------------------- 
    
    # Build Test Project
    pushd $script_root
    pushd "..\"
    $sln_name = "`"kinsol_object.sln`""
    $proj_name = "`"kinsol_test/kinsol_test.vcxproj`""
    & devenv $sln_name /deploy Release $proj_name /projectconfig Release
    popd
    popd
    
    #Run Executable
    pushd $script_root
    pushd "..\Release"
    write-host "RUNNING CASE = $tfile_name"
    Start-Process -filepath kinsol_test.exe -Wait
    write-host "--------------------------------------"
    popd
    popd
}

#WINMERGE gold <-> run
pushd "$script_root"
Start-Process WinmergeU -ArgumentList "/f *.* test_gold test_output" -Wait
popd

#WINMERGE parallel <-> serial
pushd "$script_root\test_output"
Start-Process WinmergeU -ArgumentList "/f *.* parallel serial" -Wait
popd