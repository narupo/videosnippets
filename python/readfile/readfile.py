import sys


def usage():
    print('''Read file content.

Usage:

    readfile.py [file-name]
''')


def read_file(fname):
    try:
        with open(fname, 'rt') as fin:
            return fin.read()
    except FileNotFoundError as e:
        raise e


def main():
    if len(sys.argv) < 2:
        usage()
        sys.exit(0)
    
    fname = sys.argv[1]

    try:
        content = read_file(fname)
    except FileNotFoundError as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)

    print(content, end='')


if __name__ == '__main__':
    main()
    