def parse(file_name : str):
    file_data = open(file_name)
    data = file_data.read()
    file_data.close()
        
    return {}