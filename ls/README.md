# my_ls

The language of the projet is C.

The project is partially replicating the functionality of the shell command ls.

FORMAT:
my_ls [-at] [path ...]

If no operands are given, the contents of the current directory are displayed.
The default output is sorted in the lexigraphical (ASCII) order and does not display hidden files and directories.

The supported options are:
-a Include directory entries whose names begin with a dot (.).
-t Sort by time modified (most recently modified first) before sorting the operands by lexicographical order.
