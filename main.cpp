#include "main.hpp"
const char * DIR_FILES = "/home/fedor/iss/test_zlib/files";
const char* compressed_file = "compressed.zlib";
const unsigned int LEVEL = 6;

std::vector<Bytef> fileData( files_info file )
{
    std::vector<char> data;

    std::ifstream f(file.path, std::ifstream::binary);
    data.resize(file.size_in);
    f.read(data.data(),file.size_in);
    f.close();
    std::vector<Bytef> data_new(data.begin(), data.end());

    return data_new;
}
std::streampos filesize(const char* filename)
{
    std::ifstream file(filename, std::ifstream::ate | std::ifstream::binary);
    std::streampos fsize = file.tellg();
    file.close();
    return fsize; 
}
// Получение списка логов
std::vector<files_info> getFiles( )
{
    std::string path;
    std::vector<files_info> files;
    size_t size_in = 0;
    size_t size_out = 0;
    for (const auto & entry : std::filesystem::directory_iterator(DIR_FILES))
    {
        path = entry.path();
        size_in = filesize( path.c_str() );
        size_out = size_in * 2;
        files.push_back(files_info(path,size_in,size_out));
    }
    return files; 
}
void deflateInit_status( int status )
{
    if ( status == Z_OK )
    {
        std::cout << "Инициализация начальными значениями прошла УСПЕШНО" << std::endl;
        return;
    }
    if( status == Z_MEM_ERROR )
    { 
        std::cout << "НЕУДАЧА недостаточно памяти " << std::endl;
        return;
    }
    if ( status == Z_STREAM_ERROR )
    {
        std::cout << "НЕУДАЧА неправильно выставлен уровень" << std::endl;
        return;
    }
    if ( status == Z_VERSION_ERROR )
    {
        std::cout << "НУЕДАЧА  версия библиотеки zlib несовместима с версией, на которую рассчитывает приложение" << std::endl;
        return;
    }
}
void deflate_status( int status )
{
    if ( status == Z_OK )
    {
        std::cout << "запись прошла УСПЕШНО" << std::endl;
        return;
    }
    if( status == Z_STREAM_END )
    { 
        std::cout << "обработан весь поток входных данных и все результаты сброшены в выходной буфер УСПЕШНО " << std::endl;
        return;
    }
    if ( status == Z_STREAM_ERROR )
    {
        std::cout << "НЕУДАЧА переменная потока неверно инициализирована" << std::endl;
        return;
    }
    if ( status == Z_BUF_ERROR )
    {
        std::cout << "НУЕДАЧА  нет возможности работать с буферами" << std::endl;
        return;
    }    
}
int main()
{
    z_stream stream = {0};
    std::vector<Bytef> data_in;
    std::vector<Bytef> data_out;
    std::vector<files_info> files = getFiles();
    int flush = 0;

    std::cout << "Тестовая программа для изучения zlib." << std::endl;

    //Инициализация структуры начальными значениями
    deflateInit_status(deflateInit( &stream, LEVEL ));
    
    std::ofstream f(compressed_file);

    for ( size_t i = 0; i < files.size(); ++i )
    {
        data_in = fileData(files[i]);
        std::cout << files[i].path << " size_in/size_out:" << files[i].size_in << "/" << files[i].size_out << std::endl;
        stream.avail_in = static_cast<uInt>(files[i].size_in);
        stream.next_in = data_in.data();
        stream.avail_out = static_cast<uInt>(files[i].size_out);
        data_out.resize(files[i].size_out);
        stream.next_out = data_out.data();
        flush = (i == (files.size()-1)) ? Z_FINISH : Z_NO_FLUSH;
        deflate_status(deflate(&stream,flush));
        std::cout << data_out.size() << std::endl;
        std::vector<char> data_new(data_out.begin(), data_out.end());
        size_t size = data_new.size() - stream.avail_out;
        f.write(data_new.data(),size);
        data_out.clear();
    }
    deflateEnd(&stream);
    
    return 0;
}