#include<iostream>
#include<fstream>

void swap(int i, int j, unsigned char* S) {
    //std::cout << i << "    " << j << std::endl;
    unsigned char temp = S[i];
    S[i] = S[j];
    S[j]=temp;
}

/*unsigned char* RC4m() {
	const int L = 6;
	int key[L] = { 0,0,3,0,3,5 };
	const int N = 256;
	unsigned char S[N];
	for (int i = 0; i < N; ++i) {
		S[i] = i;
	}
	int j = 0;
	for (int i = 0; i < N; ++i) {
		j = (j + S[i] + key[i % L]) % N;
        swap(i, j, S);
	}
	for (int i = (N / 2) - 1; i > -1; --i) {
		j = ((j + S[i]) xor key[i % L]) % N;
		swap(i, j, S);
	}
	for (int i = (N / 2); i < N; ++i) {
		j = ((j + S[i]) xor key[i % L]) % N;
        swap(i, j, S);
	}
	for (int y = 0; y < N; ++y) {
		int i;
		if (y % 2 == 0)  i = y / 2;
		else i = N - (y + 1) / 2;
		j = (j + S[i] + key[i % L]) % N;
        swap(i, j, S);
	}
	return S;
}*/

unsigned char PRGA(int &i, int &j, unsigned char* S) {  //генерация псевдо-случайного символа
    const int N = 256;
    i=(++i)%N;
    j = (j + S[i]) % N;
    swap(i, j, S);
    int t1 = (S[i] + S[j]) % N;
    int t2 = ((S[(i >> 3 xor i << 5) % N] + S[(i << 5 xor j >> 3) % N]) xor 0xAA) % N;
    int t3 = (j + S[j]) % N;
    unsigned char k = ((S[t1] + S[t2]) xor S[t3]) % N;
    return k;
}

int main(int argc, char* argv[]) {
	

    char ifname[256];
    char ofname[256];

    if (argc < 2) {
        std::cout << "Source: ";
        std::cin.getline(ifname, 256);
    }
    else {
        strcpy_s(ifname, argv[1]);
    }

    if (argc < 3) {
        std::cout << "Destination: ";
        std::cin.getline(ofname, 256);
    }
    else {
        strcpy_s(ofname, argv[2]);
    }


    std::ifstream inFile(ifname);
    if (!inFile) {
        std::cout << "Can not open file " << ifname << std::endl;
        return 1;
    }
    std::ofstream outFile(ofname);
    if (!inFile) {
        std::cout << "Can not open file " << ofname << std::endl;
        inFile.close();
        return 2;
    }
    unsigned char ch;
    //unsigned char* S = RC4m();

    //--------------------инициализация RC4m-------------------
    const int L = 6;
	int key[L] = { 0,0,3,0,3,5 };
	const int N = 256;
	unsigned char S[N];
	for (int i = 0; i < N; ++i) {
		S[i] = i;
	}
	int j = 0;
	for (int i = 0; i < N; ++i) {
		j = (j + S[i] + key[i % L]) % N;
        swap(i, j, S);
	}
	for (int i = (N / 2) - 1; i > -1; --i) {
		j = ((j + S[i]) xor key[i % L]) % N;
		swap(i, j, S);
	}
	for (int i = (N / 2); i < N; ++i) {
		j = ((j + S[i]) xor key[i % L]) % N;
        swap(i, j, S);
	}
	for (int y = 0; y < N; ++y) {
		int i;
		if (y % 2 == 0)  i = y / 2;
		else i = N - (y + 1) / 2;
		j = (j + S[i] + key[i % L]) % N;
        swap(i, j, S);
	}
    //----------------конец инициализации-------------------


    int i = 0, j = 0; //инициализация генератора

    while (inFile.peek() != EOF) {
        ch = inFile.get();
        unsigned char k = PRGA(i, j, S);//генерация случайного символа
        ch = ch xor k; //шифрование символа
        
        outFile.put(ch);
    }
    inFile.close();
    outFile.close();
    std::cout << "Done!" << std::endl;
}