
def _try_load_file(file_name : str):
    try:
        file_handler = open(file_name, "r")
        file_data = file_handler.read()
        return file_data
    except FileNotFoundError:
        return None

def assert_files(self, real, expected):
    
    real_file_data = _try_load_file(real)
    expected_file_data = _try_load_file(expected)
    
    self.assertIsNotNone(real_file_data)
    self.assertIsNotNone(expected_file_data)
        
    self.assertEqual(real_file_data, expected_file_data)
    