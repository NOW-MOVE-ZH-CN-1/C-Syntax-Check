#include <iostream>
#include <fstream>
#include <string>
#include <vector>




std::string trim(const std::string &s)
{
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}


void scan_file(const std::string &filepath)
{
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cerr << "[FATAL] Cannot open file: " << filepath << std::endl;

        return;
    }

    std::string line;
    int line_no = 0;
    int brace_cnt = 0;
    int paren_cnt = 0;
    bool in_block_comment = false;

    while (getline(fin, line))
    {
        line_no++;
        std::string buf = line;
        size_t i = 0;

        while (i < buf.size())
        {
            if (in_block_comment)
            {
                if (buf.substr(i,2) == "*/")
                {
                    in_block_comment = false;
                    i +=2;
                }else{
                    i++;
                }
                continue;
            }

            
            if(buf.substr(i,2) == "/*")
            {
                in_block_comment = true;
                i +=2;
                continue;
            }
            
            if(buf.substr(i,2) == "//")
            {
                break;
            }

            char c = buf[i];
            if(c == '{') brace_cnt++;
            if(c == '}') brace_cnt--;
            if(c == '(') paren_cnt++;
            if(c == ')') paren_cnt--;
            i++;
        }

        
        std::string t = trim(line);
        if (!t.empty())
        {
            char last = t.back();
            if (last != ';' && last != '{' && last != '}' && last != ',')
            {
                
                if (t[0] != '#' && t.find("//") == std::string::npos)
                {
                    
                    if (!(t.find("if")==0 || t.find("for")==0 || t.find("while")==0))
                    {
                        std::cout << "[CE WARN] L" << line_no << ": Possible missing `;`" << std::endl;
                    }
                }
            }
        }
    }
    fin.close();

    if(in_block_comment)
    {
        std::cout << "[FATAL CE]: Unclosed block comment /* */" << std::endl;
    }
    if(brace_cnt !=0)
    {
        std::cout << "[FATAL CE]: Brace {} mismatch, balance=" << brace_cnt << std::endl;
    }
    if(paren_cnt !=0)
    {
        std::cout << "[FATAL CE]: Parentheses () mismatch, balance=" << paren_cnt << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <source-file>" << std::endl;
        return 1;
    }
    scan_file(argv[1]);
    return 0;
}
