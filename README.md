Assignment 1


Question 1)

I)Requirement to execute programm: 
  1)terminal should open at 2024201077_A1 folder. 
  2)Assignment1 folder must be not present in 2024201077_A1 folder, otherwise pgogram throw an error.
  3)Input file must be present in 2024201077_A1 program.

II)instructions to execute code:
  in terminal: line1->g++ 2024201077_A1_Q1.cpp     
                    ->./a.out <input file name> <flag> (either 0 or 1) <start_index> <end_index>                

III) Working procedure of code:
   ->code first check directory is present or not if present then  throw error or create new directory, after that open input file and, and check wether the   flag is perfrt or not, if it is perfect then acording to flag control goes to if condition, now for flag 0 the complate file is devided in 1MB chunk, and then reverse this chunk and copy to output file, for flag 1, take starting and ending index and for first part and reverse (0 to startindex-1) file by deviding and reversing each chunk, after that copy (startindex to endindex) in output file and then, reverse file from endIndex+1 to end file.




Question 2)
I)Requirement to execute programm: 
  1)terminal should open at 2024201077_A1 folder. 
  2)if we have input file name Sample.txt then only give /Sample.txt in path, for new file give /Assignment1/<newfile_name> and for folder give /Assignment1

II)instruction to execute code:
in terminal: line1->g++ 2024201077_A2_Q2.cpp
                  ->./a.out <newfile_path> <oldfile_path> <directory_path>   //Ex: ./a.out /Assignment1/0_Sample.txt /Sample.txt /Assignment1


III)Working procedure of code:
   ->code first check directory is created or not and then open two file and then check is both file are reverse of each other or not by compering one by one character, it will take some time for large file. after that program check file size and after that it check permission for new file,old file anf directory.