class Application:
    def __init__(self,title_header,w_header,h_header):
        self.title=title_header

    def print_header(self):
        print(f'|{"-"*max(len(self.title)),self.w_header}|')