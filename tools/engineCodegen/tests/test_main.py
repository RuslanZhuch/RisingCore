import unittest
from pathlib import Path

def _remove_dest_files():
    [f.unlink() for f in Path("dest").glob("*") if f.is_file()] 

if __name__ == '__main__':
    _remove_dest_files()
    
    testsuite = unittest.TestLoader().discover('.')
    unittest.TextTestRunner(verbosity=1).run(testsuite)