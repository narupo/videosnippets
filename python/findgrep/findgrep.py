import os
import sys


def usage():
    print('''Find and grep files.

Usage:

    findgrep [directory] [search-string]
''')


def main():
    if len(sys.argv) < 3:
        usage()
        sys.exit(0)

    dirpath = sys.argv[1]
    findstr = sys.argv[2]

    if not os.path.exists(dirpath):
        print('ディレクトリが存在しません。', file=sys.stderr)
        sys.exit(1)
    
    for curdir, dirs, files in os.walk(dirpath):
        for fname in files:
            if findstr in fname:
                path = os.path.abspath(os.path.join(curdir, fname))
                print(path)


if __name__ == '__main__':
    main()
