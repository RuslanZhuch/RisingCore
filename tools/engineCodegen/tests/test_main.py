import unittest
import shutil

import os

def _remove_dest_files():
    if os.path.exists('dest'):
        shutil.rmtree('dest')
        os.makedirs('dest')

if __name__ == '__main__':
    _remove_dest_files()
    
    testsuite = unittest.TestLoader().discover('.')
    unittest.TextTestRunner(verbosity=1).run(testsuite)
    