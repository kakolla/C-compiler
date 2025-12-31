#include <fstream>
#include <string>
#include <iostream>


std::string compile_program(int x) {
    std::string res = "mov w0, #" + std::to_string(x) + "\nret\n";
    std::cout << res << std::endl;
    return res;

}

std::string compile_program(char x) {
    return "mov w0, #" + std::to_string((int)x) + "\nret\n";
} 

std::string compile_program(bool x) {
    std::string res = "mov w0, #" + std::to_string(static_cast<int>(x)) + "\nret\n";
    std::cout << res << std::endl;
    return res;
}




int main() {
    // std::string asm_code = compile_program(31415); // generate assembly code in a file
    std::string asm_code = compile_program('K');
    std::ofstream output_stream("c_entry.s");
    output_stream << ".globl _c_entry\n"
        << "_c_entry:\n"
        << asm_code;
    
}