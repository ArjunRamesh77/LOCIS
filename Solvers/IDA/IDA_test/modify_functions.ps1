# This script creates a modiefied functions.cpp file for kinsol_test project
$num_max_threads = 100

$script_root = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

# Modifying files
$functions_cpp =  "$script_root\functions.cpp"
$local_func_prototypes_h =  "$script_root\local_func_prototypes.h"
$thread_data_h =  "$script_root\thread_data.h"
$extra_headers_h =  "$script_root\extra_headers.h"

$test_files = Get-Childitem -Path "$script_root\test_cases" -Name -include "*.idat"
$tfout_path = "$script_root\test_output"


# Main loop area
for($n = 0; $n -lt $test_files.Count; $n++)
{

    $tfile_name = $test_files[$n]
    $test_file = Get-Content "$script_root\test_cases\$tfile_name"
    write-host "BUILDING CASE = $tfile_name"
        
    $output = "$tfout_path\$tfile_name"

    #Write header section
    write-output "" | out-file $functions_cpp -encoding ascii 
    write-output "" | out-file $local_func_prototypes_h -encoding ascii 
    write-output "" | out-file $thread_data_h -encoding ascii 
    write-output "" | out-file $extra_headers_h -encoding ascii 

#---------------------------------------------------------------------- MODIFY FILE SECTION -------------------------------------------------------------------------------

    write-output '#include <iostream>' | out-file $functions_cpp -encoding ascii -append
    write-output '#include <fstream>' | out-file $functions_cpp -encoding ascii -append
    write-output '#include "ida_helpers.h"' | out-file $functions_cpp -encoding ascii -append
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
    write-output 'void JacFunc(double t, double *y, double *yp, double **J, double cj);' | out-file $functions_cpp -encoding ascii -append

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
                    $func_header = "void SetSize_$nt(int &NEQ) {"
                }
                elseif($inter_name -eq "SETGUESS")
                {
                   $func_header = "void SetGuess_$nt(int NEQ, double* guess_y, double* guess_yp) {"
                }
                elseif($inter_name -eq "SETOPTIONS")
                {
                    $func_header = "void SetOptions_$nt(IDA_HELPERS::IDAOptions *options, int NEQ) {"   
                }
                elseif($inter_name -eq "RESFUNC")
                {
                    $func_header = "void ResFunc_$nt(double t, double *y, double *yp, double* r) {"    
                }
                elseif($inter_name -eq "JACFUNC")
                {
                    $func_header = "void JacFunc_$nt(double t, double *y, double *yp, double **J, double cj) {"
                }
                elseif($inter_name -eq "INFO")
                {
                    $func_header = "int UserInfoHandler_$nt(void *s, int type) {" 
                    
                    # basic 
                	$func_data.Add("     if (type == 0) {")
                	$func_data.Add("	     IDA_HELPERS::infomsgs* info = (IDA_HELPERS::infomsgs*)s;")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> IDA OBJECT INFO `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     LastOrder :: `" + to_string(info->LastOrder));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     Numsteps :: `" + to_string(info->Numsteps));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     LastStep :: `" + to_string(info->LastStep));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     t :: `" + to_string(info->t));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     Solution(yval)::`");")
                    $func_data.Add("         for (int i = 0; i < info->NEQ; i++) ")
                    $func_data.Add("              THREAD_OUTPUT$nt.push_back(`"        yval[`" + to_string(i) + `"] = `" + to_string(info->yval[i]));")
                    $func_data.Add("         for (int i = 0; i < info->NEQ; i++) ")
                    $func_data.Add("              THREAD_OUTPUT$nt.push_back(`"        ypval[`" + to_string(i) + `"] = `" + to_string(info->yp_val[i]));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     numroots :: `" + to_string(info->numroots));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }");  
                    $func_data.Add("     else if (type == 1)")
                	$func_data.Add("     {");
                	$func_data.Add("	     IDA_HELPERS::finalstatsl1* fs1 = (IDA_HELPERS::finalstatsl1*)s;");
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> IDA FINAL STATISTICS 1 `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     NEQ :: `" + to_string(fs1->NEQ));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumSteps :: `" + to_string(fs1->nst));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumResEvals :: `" + to_string(fs1->nre));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDADlsGetNumJacEvals :: `" + to_string(fs1->nje));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumNonlinSolvIters :: `" + to_string(fs1->nni));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumErrTestFails :: `" + to_string(fs1->netf));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumNonlinSolvConvFails :: `" + to_string(fs1->ncfn));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDADlsGetNumResEvals :: `" + to_string(fs1->nreLS));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumGEvals :: `" + to_string(fs1->nge));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }")
                	$func_data.Add("     else if (type == 2)")
                	$func_data.Add("     {")
                	$func_data.Add("	     IDA_HELPERS::finalstatsl2* fs2 = (IDA_HELPERS::finalstatsl2*)s;")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> IDA FINAL STATISTICS 2 `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     NEQ :: `" + to_string(fs2->NEQ));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumSteps :: `" + to_string(fs2->nst));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumResEvals :: `" + to_string(fs2->nre));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDADlsGetNumJacEvals :: `" + to_string(fs2->nje));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumNonlinSolvIters :: `" + to_string(fs2->nni));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumErrTestFails :: `" + to_string(fs2->netf));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumNonlinSolvConvFails :: `" + to_string(fs2->ncfn));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDADlsGetNumResEvals :: `" + to_string(fs2->nreLS));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumGEvals :: `" + to_string(fs2->nge));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetWorkSpace(lenrw) :: `" + to_string(fs2->lenrw));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetWorkSpace(leniw) :: `" + to_string(fs2->leniw));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumErrTestFails :: `" + to_string(fs2->netfails));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetLastOrder :: `" + to_string(fs2->klast));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetNumLinSolvSetups :: `" + to_string(fs2->nlinsetups));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetCurrentOrder :: `" + to_string(fs2->kcur));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetLastStep :: `" + to_string(fs2->hlast));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetCurrentStep :: `" + to_string(fs2->hcur));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetActualInitSte :: `" + to_string(fs2->hinused));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetCurrentTime :: `" + to_string(fs2->tcur));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetTolScaleFactor :: `" + to_string(fs2->tolsfac));")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"  `");")
                	$func_data.Add("     }")
                    $func_data.Add("     else if (type == 3)")
                	$func_data.Add("     {")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     >> IDA CONSISTENT CALC RESULT `");")
                    $func_data.Add("	     IDA_HELPERS::finalstatsl1* fs1 = (IDA_HELPERS::finalstatsl1*)s;");
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetConsistentIC :: yval0`");")
                    $func_data.Add("         for (int i = 0; i < fs1->NEQ; i++) ")
                    $func_data.Add("              THREAD_OUTPUT$nt.push_back(`"        yval0[`" + to_string(i) + `"] = `" + to_string(fs1->Init_cond_y[i]));")
                    $func_data.Add("	     THREAD_OUTPUT$nt.push_back(`"     IDAGetConsistentIC :: ypval0`");")
                    $func_data.Add("         for (int i = 0; i < fs1->NEQ; i++) ")
                    $func_data.Add("              THREAD_OUTPUT$nt.push_back(`"        ypval0[`" + to_string(i) + `"] = `" + to_string(fs1->Init_cond_yp[i]));")
                    $func_data.Add("     }")
                    $func_data.Add("     return 0;");
                      
                }
                elseif($inter_name -eq "ERR")
                {
                    $func_header = "int UserErrorHandler_$nt(void *s, int type) {"
                    
                    # basic
                    $func_data.Add("     if (type == 0 || type == 1 || type == 2) {")
                	$func_data.Add("          IDA_HELPERS::error* erro = (IDA_HELPERS::error*)s;")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`" `");")
                	$func_data.Add("          THREAD_OUTPUT$nt.push_back(`"     >> IDA ERROR `");")
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
                write-output "void RunIDA_$nt(IDA_HELPERS::f_Init funcinit, IDA_HELPERS::f_setdata funcsetdata, IDA_HELPERS::f_setoptions funcsetoptions, IDA_HELPERS::f_funcisolve funcsolve, IDA_HELPERS::f_free funcfree) {" | out-file $functions_cpp -encoding ascii -append
                write-output "void RunIDA_$nt(IDA_HELPERS::f_Init funcinit, IDA_HELPERS::f_setdata funcsetdata, IDA_HELPERS::f_setoptions funcsetoptions, IDA_HELPERS::f_funcisolve funcsolve, IDA_HELPERS::f_free funcfree);" | out-file $local_func_prototypes_h -encoding ascii -append
                write-output "     int objnum;" | out-file $functions_cpp -encoding ascii -append
                write-output "     int NEQ;" | out-file $functions_cpp -encoding ascii -append
                write-output "     THID[$($nt - 1)] = std::this_thread::get_id();" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     IDA_HELPERS::error_IDA* errorvector = new IDA_HELPERS::error_IDA;" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     SetSize_$nt(NEQ);" | out-file $functions_cpp -encoding ascii -append
                write-output "     double* guess_y = new double[NEQ];" | out-file $functions_cpp -encoding ascii -append
                write-output "     double* guess_yp = new double[NEQ];" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     SetGuess_$nt(NEQ, guess_y, guess_yp);" | out-file $functions_cpp -encoding ascii -append
                
                #Write Guess to global vector
                write-output "     THREAD_OUTPUT$nt.push_back(`" `");" | out-file $functions_cpp -encoding ascii -append
                write-output "     THREAD_OUTPUT$nt.push_back(`">> GUESS FOR SYSTEM(y)`");" | out-file $functions_cpp -encoding ascii -append
                write-output "     for (int i = 0; i < NEQ; i++)" | out-file $functions_cpp -encoding ascii -append
                write-output "          THREAD_OUTPUT$nt.push_back(`"     y[`" + std::to_string(i) + `"] = `" + std::to_string(guess_y[i]));" | out-file $functions_cpp -encoding ascii -append
                write-output "     THREAD_OUTPUT$nt.push_back(`">> GUESS FOR SYSTEM(yp)`");" | out-file $functions_cpp -encoding ascii -append
                write-output "     for (int i = 0; i < NEQ; i++)" | out-file $functions_cpp -encoding ascii -append
                write-output "          THREAD_OUTPUT$nt.push_back(`"     yp[`" + std::to_string(i) + `"] = `" + std::to_string(guess_yp[i]));" | out-file $functions_cpp -encoding ascii -append

                
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     IDA_HELPERS::IDAOptions* options = new IDA_HELPERS::IDAOptions;" | out-file $functions_cpp -encoding ascii -append
                write-output "     SetOptions_$nt(options, NEQ);" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                write-output "     objnum = funcinit(0);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(objnum < 0) {"                                                                                                                         | out-file $functions_cpp -encoding ascii -append
                write-output "          IDA_HELPERS::error* erro = new IDA_HELPERS::error; "                                                                                 | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                            | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                              | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_INIT returned with a fatal error, program will stop`"); "                                             | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                | out-file $functions_cpp -encoding ascii -append

                write-output "     int setdata_rc = funcsetdata(objnum, JacFunc, ResFunc_$nt, UserErrorHandler_$nt, UserInfoHandler_$nt, NULL, errorvector, NEQ, guess_y, guess_yp);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(setdata_rc < 0) {"                                                                                                                              | out-file $functions_cpp -encoding ascii -append
                write-output "          IDA_HELPERS::error* erro = new IDA_HELPERS::error; "                                                                                          | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                                     | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                       | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SETDATA returned with a fatal error, program will stop`"); "                                                   | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                         | out-file $functions_cpp -encoding ascii -append

                write-output "     int setoptions_rc = funcsetoptions(objnum, options);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(setoptions_rc <= 0) {"                                                                                                                          | out-file $functions_cpp -encoding ascii -append
                write-output "          IDA_HELPERS::error* erro = new IDA_HELPERS::error; "                                                                                          | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                                     | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                       | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SETOPTIONS returned with a fatal error, program will stop`"); "                                                | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                         | out-file $functions_cpp -encoding ascii -append

                write-output "     int solve_rc = funcsolve(objnum);" | out-file $functions_cpp -encoding ascii -append
                write-output "     if(solve_rc <= 0) {"                                                                                                                          | out-file $functions_cpp -encoding ascii -append
                write-output "          IDA_HELPERS::error* erro = new IDA_HELPERS::error; "                                                                                     | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrCode = objnum; "                                                                                                                | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrType = `"Unexpected Error`"; "                                                                                                  | out-file $functions_cpp -encoding ascii -append
                write-output "          erro->ErrLines.push_back(`"DLL_SOLVE returned with a fatal error, program will stop`"); "                                                | out-file $functions_cpp -encoding ascii -append
                write-output "          UserErrorHandler_$nt(erro, 0); delete erro; return;}"                                                                                    | out-file $functions_cpp -encoding ascii -append

                write-output "     funcfree(objnum);" | out-file $functions_cpp -encoding ascii -append
                write-output "     " | out-file $functions_cpp -encoding ascii -append
                     
                write-output "     delete options;" | out-file $functions_cpp -encoding ascii -append
                write-output "     delete errorvector;" | out-file $functions_cpp -encoding ascii -append
                write-output "}" | out-file $functions_cpp -encoding ascii -append
                $got_thread = 0
            }
        }
    }

    # Write Run Section
    write-output "//------------------------------------------ RUN ;)------------------------------------------" | out-file $functions_cpp -encoding ascii -append
    write-output "void RunTest(IDA_HELPERS::f_Init funcinit, IDA_HELPERS::f_setdata funcsetdata, IDA_HELPERS::f_setoptions funcsetoptions, IDA_HELPERS::f_funcisolve funcsolve, IDA_HELPERS::f_free funcfree) {" | out-file $functions_cpp -encoding ascii -append
    write-output "" | out-file $functions_cpp -encoding ascii -append

        write-output "     //PARALLEL TEST" | out-file $functions_cpp -encoding ascii -append
        # Write Thread spawns ( Parallel section)
        for($i = 1; $i -le $nt; $i++)
        {
            write-output "     std::thread THREAD$i(RunIDA_$i, funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);" | out-file $functions_cpp -encoding ascii -append

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
            write-output "     std::thread THREAD$j(RunIDA_$i, funcinit, funcsetdata, funcsetoptions, funcsolve, funcfree);" | out-file $functions_cpp -encoding ascii -append
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
    write-output "void JacFunc(double t, double *y, double *yp, double **J, double cj) {" | out-file $functions_cpp -encoding ascii -append
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
        
        write-output "          JacFunc_$($i + 1)(t, y, yp, J, cj);" | out-file $functions_cpp -encoding ascii -append       
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
    $sln_name = "`"ida_object.sln`""
    $proj_name = "`"ida_test/ida_test.vcxproj`""
    & devenv $sln_name /deploy Release $proj_name /projectconfig Release
    popd
    popd
    

    #Run Executable
    pushd $script_root
    pushd "..\Release"
    write-host "RUNNING CASE = $tfile_name"
    Start-Process -filepath ida_test.exe -Wait
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

