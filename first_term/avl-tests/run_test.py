# Python 2.7 required

import sys
import os
import os.path
import subprocess

def main():
    if len(sys.argv) <= 1:
        print 'Run on Windows: python run_test.py solution.exe [test_name]'
        print 'Run on *nix:    python run_test.py ./solution [test_name]'
        sys.exit(2)
    target_bin = sys.argv[1]
    if len(sys.argv) == 3:
        test_names = [sys.argv[2]]
    else:
        test_names = sorted(os.listdir('tests-in'))
    for test in test_names:
        print test
        with open(os.path.join('tests-in', test), 'rb') as test_file:
            stud_ans = subprocess.check_output([target_bin], stdin=test_file)
        stud_ans_s = stud_ans.split()
        with open(os.path.join('tests-ans', test)) as ans_file:
            ans = ans_file.read().split()
        if stud_ans_s != ans:
            print 'WA'
            with open('wa-stud.txt', 'wb') as stud_wa_file:
                stud_wa_file.write(stud_ans)
            sys.exit(1)
    print
    print 'OK'

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
