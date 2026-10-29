#include <iostream>

using namespace std;

void send_CGI_header(){
    cout << "Content-type: text/html\n";
    cout << "\n"; // this blank line is a necessary part of the CGI protocol.
}
void send_contents(){
    cout << "<!doctype html>\n";
    cout << "<html>\n";
    cout << "<title>Hello</title>\n";
    cout << "<p>Hello, world!\n";
    // C strings can, with care, contain double-quote characters, newlines, etc.
    cout << "<p><a href=\"http://validator.w3.org/check?uri=referer\">";
    cout << "Valid.</a>\n";
    cout << "</html>\n";
    cout << endl;
}

int main(){
    send_CGI_header();
    send_contents();
    return 0;
}