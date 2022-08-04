int main()
{
    SimpleFileWriter file_writer("a_file");

    for(size_t k = 0; k < 10; k++)
    {
        float f = k;
        file_writer.appendStaticType(f);
    }
}

// ##################################################################
// ##################################################################
// ##################################################################

int main()
{
    logging::openFile("variable0")
}

void aFunction()
{
    float value;
    // ...
    logging::logToFile("variable0", value);
}

// ##################################################################
// ##################################################################
// ##################################################################

int main()
{
    logging::openFile("FILE_FUNCTION_X")
}

void functionX(const float q, const double x, const int a, const std::string& s, const std::vector<uint64_t> vec)
{
    logging::appendToFile("FILE_FUNCTION_X", value);
}

void functionXRead(const float _q, const double _x, const int _a, const std::string& _s, const std::vector<uint64_t> _vec)
{
    float q;
    double x;
    int a;
    std::string s;
    std::vector<uint64_t> vec;

    // logging::readFromFile("FILE_FUNCTION_X", q, x, a, s, vec);
    READ_FROM_FILE(q, x, a, s, vec)
}

#define READ_FROM_FILE(...) logging::readFromFile(__FILE__, __func__, __VA_ARGS__)

template <typename ...Us>
void readFromFile(const std::string& file_name, const std::string& function_name, Us& parameters)
{

}










