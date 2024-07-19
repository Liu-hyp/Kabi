
import getopt
import traceback
from string import Template

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
    except Exception as e:
        print("Failed to generate kabi rpc server, err:" + e)
        trace.print_exc()
        print('=' * 150)

if __name__ == '__main__':
    generator_project()

