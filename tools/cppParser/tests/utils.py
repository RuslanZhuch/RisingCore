
def assert_files(self, real, expected):
    real_file = open(real, "r")
    real_file_data = real_file.read()
    
    expected_file = open(expected, "r")
    expected_file_data = expected_file.read()
    self.assertEqual(real_file_data, expected_file_data)