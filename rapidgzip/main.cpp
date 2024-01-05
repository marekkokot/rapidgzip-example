#include <iostream>
#include <ParallelGzipReader.hpp>
#include <Standard.hpp>
#include <vector>
using namespace std;

//compile command:
//g++ -std=c++20 -O3 -I src/rapidgzip  -I src/core -fconstexpr-ops-limit=99000100 main.cpp -lz
int main(int argc, char** argv) {

	if (argc < 4) {
		std::cerr << "Usage: " << argv[0] << " <input_file> <n_threads> <reader_chunk_size_in_MB>\n";
		return 1;
	}
	UniqueFileReader file_reader = std::make_unique<StandardFileReader>(argv[1]);
	const size_t n_threads = std::atoi(argv[2]);
	const size_t reader_chunk_size = std::atoi(argv[3]) * (1ull << 20);

	rapidgzip::ParallelGzipReader<> reader(std::move(file_reader), n_threads, reader_chunk_size);

	//reader.setCRC32Enabled( true );

	const size_t chunk_size = 1ull << 24;
	std::vector<char> chunk(chunk_size);

	while (true) {
		auto R = reader.read(chunk.data(), chunk_size);
		if (!R)
			break;
		std::cout.write(chunk.data(), R);
	}

	std::cerr << "eof?: " << reader.eof() << "\n";
	return 0;
}