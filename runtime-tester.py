import sys
import os
import subprocess
import time

SEGMENT_COUNT_INDEX = 2

def count_segments(input_file_path):
    h = 0
    v = 0
    with open(input_file_path, 'r') as file:
        for line in file:
            if line.startswith('h'):
                h = line.split()[SEGMENT_COUNT_INDEX]
            elif line.startswith('v'):
                v = line.split()[SEGMENT_COUNT_INDEX]

    return (h,v)

def main():
    input_folder_path = sys.argv[1]
    executable_path = sys.argv[2]

    output_file = 'runtime-tester-output-{}-{}.csv'.format(os.path.basename(input_folder_path), os.path.basename(executable_path))

    with open(output_file, 'w') as file:
        file.write('horizontal segments,vertical segments,runtime(ms)\n')

    input_files = os.listdir(input_folder_path)
    input_files.sort()

    for file in input_files:
        file_path = os.path.join(input_folder_path, file)
        h,v = count_segments(file_path)

        print('running program {} with input file {}'.format(os.path.basename(executable_path), file))

        process = subprocess.Popen(['./{}'.format(executable_path)], 
                           stdout=subprocess.PIPE,
                           stdin=subprocess.PIPE)

        with open(file_path, 'rb') as file:
            start = time.time()
            process.communicate(file.read())
            end = time.time()
        execution_time = (end - start) * 1000
        print('execution time: {} ms'.format(execution_time))
        with open(output_file, 'a') as file:
            file.write('{},{},{}\n'.format(h,v,execution_time))

if __name__ == '__main__':
    main()