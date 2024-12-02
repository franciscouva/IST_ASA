for file in tests/tests/testfile*;
do
    echo "-----------------------";
    echo $file >> time_tests.txt;
    { time  python3 proj3.py < $file; } 2>> time_tests.txt;
    echo "-----------------------";
done