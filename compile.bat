:: SET boostlibpath=C:\studium\semester08\adbs\TUD_ADBS_SS12_Timebased\boost_1_49_0\stage\lib;
:: SET PATH=%PATH%%boostlibpath%  
:: SET PATH
g++ *.h *.cpp -I boost_1_49_0 -std=c++0x -o main -L boost_1_49_0/stage/lib -l boost_date_time-mgw46-mt-d-1_49 -l boost_thread-mgw46-mt-d-1_49 && main