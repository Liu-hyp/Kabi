#!/usr/bin/python3.10
import getopt
import traceback
import sys
import os
from string import Template
from datetime import datetime

project_name = ""
proto_file = ""
out_project_path = "./"
bin_path = ""
conf_path = ""
test_client_path = ""
test_client_tool_path = ""
src_path = ""

generator_path = sys.path[0]
def parseInput():
    opts,args = getopt.getopt(sys.argv[1:], "hi:o:", longopts=["help", "input=", "output="])
    for opts,arg in opts:

        if opts=="-h" or opts=="--help":
            printHelp()
            sys.exit(0)
        if opts=="-i" or opts=="--input":
            global proto_file 
            proto_file = arg 
        elif opts=="-o" or opts=="--output_path":
            global out_project_path
            out_project_path = arg
            if out_project_path[-1] != '/':
                out_project_path = out_project_path + '/'
        else:
            raise Exception("invalid options: [" + opts + ": " + arg + "]")
    if not os.path.exists(proto_file):
        raise Exception("Generate error, not exist protobuf file: " + proto_file)

    if ".proto" not in proto_file:
        raise Exception("Generate error, input file is't standard protobuf file:[" + proto_file + "]")

    global project_name
    project_name = proto_file[0: -6]
    print("project name is " + project_name)

def printHelp():
    print('=' * 100)
    print('Welcome to use Kabi Generator, this is help document:\n')
    print('Run Environment: Python(version 3.6 or high version is better).')
    print('Run Platform: Linux Only(kernel version >= 3.9 is better).')
    print('Others: Only protobuf3 support, not support protobuf2.\n')
    print('Usage:')
    print('kabi_generator.py -[options][target]\n')
    print('Options:')
    print('-h, --help')
    print(('    ') + 'Print help document.\n')

    print('-i xxx.proto, --input xxx.proto')
    print(('    ') + 'Input the target proto file, must standard protobuf3 file, not support protobuf2.\n')

    print('-o dir, --output dir')
    print(('    ') + 'Set the path that your want to generate project, please give a dir param.\n')

    print('')
    print('For example:')
    print('kabi_generator.py -i order_server.proto -o ./')

    print('')
    print('=' * 100)  

def generator_project():
    try:
        parseInput()
        print('=' * 150)
        print("Begin to generate kabi rpc server") 
        generate_dir()
        generate_pb()
        generate_makefile()
    except Exception as e:
        print("Failed to generate kabi rpc server, err:" + e)
        trace.print_exc()
        print('=' * 150)

def generate_dir():
    print('=' * 100)
    print('Begin to generate project dir')

    if out_project_path == "":
        proj_path = './' + project_name.strip()
    if out_project_path[-1] == '/':
        proj_path = out_project_path + project_name.strip()
    else:
        proj_path = out_project_path + './' + project_name.strip()
    
    global bin_path 
    bin_path = proj_path + '/bin'

    global conf_path
    conf_path = proj_path + '/conf'

    global test_client_path 
    test_client_path = proj_path + '/test_client'

    global test_client_tool_path 
    test_client_tool_path = test_client_path + '/test_tool'

    log_path = proj_path + '/log'
    lib_path = proj_path + '/lib'
    obj_path = proj_path + '/obj'

    global src_path
    src_path = proj_path + '/' + project_name
    src_interface_path = src_path + '/interface'
    src_service_path = src_path + '/service'
    src_pb_path = src_path + '/pb'
    src_stubs_path = src_path + '/stubs'
    src_comm_path = src_path + '/comm'

    dir_list = []
    dir_list.append(proj_path) 
    dir_list.append(bin_path) 
    dir_list.append(conf_path) 
    dir_list.append(log_path) 
    dir_list.append(lib_path) 
    dir_list.append(obj_path) 
    dir_list.append(test_client_path) 
    dir_list.append(test_client_tool_path) 
    dir_list.append(src_path) 
    dir_list.append(src_interface_path) 
    dir_list.append(src_service_path) 
    dir_list.append(src_pb_path) 
    dir_list.append(src_stubs_path) 
    dir_list.append(src_comm_path) 

    for each in dir_list:
        if not os.path.exists(each):
            os.mkdir(each)
            print("succ make dir in " + each)

    print('End generate project dir')
    print('=' * 100)

def generate_pb():
    print('=' * 100)
    print('Begin generate protobuf file')
    pb_path = src_path + '/pb/'
    cmd = 'cp -r ' + proto_file + ' ' + pb_path
    cmd += ' && cd ' + pb_path + ' && protoc --cpp_out=./ ' + proto_file 
    print('excute cmd: ' + cmd)

    if os.system(cmd) != 0:
        raise Exception("execute cmd failed [" + cmd + "]")
    
    print('End generate protobuf file')
    print('=' * 100)

def generate_makefile():
    print('=' * 100)
    print('Begin to generate CmakeLists.txt')
    out_file = src_path + '/CmakeLists.txt'
    if os.path.exists(out_file):
        print('CmakeLists exist, skip generate')
        print('End generate CmakeLists')
        print('=' * 100)
        return
    template_file = open(generator_path + '/template/CmakeLists.txt.template','r')
    tmpl = Template(template_file.read())

    content = tmpl.safe_substitute(
        PROJECT_NAME = project_name,
        CREATE_TIME = datetime.now().strftime('%Y-%m-%d %H:%M:%S'))

    file = open(out_file, 'w')
    file.write(content)
    file.close()
    print('succ write to ' + out_file)
    print('End generate CmakeLists')
    print('=' * 100)

if __name__ == '__main__':
    generator_project()

