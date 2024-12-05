#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int SAMPLE_RATE = 44100; // Taxa de amostragem (Hz)
const int DURATION = 20;       // Duração da música (segundos) - agora 20 segundos
const int AMPLITUDE = 127;     // Amplitude máxima (8 bits)

// Função para gerar onda quadrada (violão)
void generateSquareWave(int* wave, double frequency, int startSample, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        wave[startSample + i] = (i % static_cast<int>(SAMPLE_RATE / frequency) < SAMPLE_RATE / (2 * frequency)) ? AMPLITUDE : -AMPLITUDE;
    }
}

// Função para gerar onda triangular (piano)
void generateTriangleWave(int* wave, double frequency, int startSample, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        double value = (2 * AMPLITUDE / M_PI) * asin(sin(2 * M_PI * frequency * i / SAMPLE_RATE));
        wave[startSample + i] = static_cast<int>(value);
    }
}

// Função para salvar o arquivo WAV
void saveWav(const string& filename, int* wave, int numSamples) {
    ofstream out(filename, ios::binary);

    // Cabeçalho do WAV
    out.write("RIFF", 4);
    int chunkSize = 36 + numSamples;
    out.write(reinterpret_cast<const char*>(&chunkSize), 4);
    out.write("WAVE", 4);

    // Subchunk1: "fmt "
    out.write("fmt ", 4);
    int subchunk1Size = 16;
    out.write(reinterpret_cast<const char*>(&subchunk1Size), 4);
    short audioFormat = 1; // PCM
    out.write(reinterpret_cast<const char*>(&audioFormat), 2);
    short numChannels = 1; // Mono
    out.write(reinterpret_cast<const char*>(&numChannels), 2);
    out.write(reinterpret_cast<const char*>(&SAMPLE_RATE), 4);
    int byteRate = SAMPLE_RATE * numChannels;
    out.write(reinterpret_cast<const char*>(&byteRate), 4);
    short blockAlign = numChannels;
    out.write(reinterpret_cast<const char*>(&blockAlign), 2);
    short bitsPerSample = 8; // 8 bits
    out.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // Subchunk2: "data"
    out.write("data", 4);
    int subchunk2Size = numSamples;
    out.write(reinterpret_cast<const char*>(&subchunk2Size), 4);

    // Dados de áudio
    for (int i = 0; i < numSamples; ++i) {
        char sample = static_cast<char>(wave[i] + 128); // Converte para unsigned 8 bits
        out.write(&sample, 1);
    }

    out.close();
}

int main() {
    // Frequências das notas da melodia de Tetris
    double frequencies[] = {440.0, 493.88, 523.25, 587.33, 659.25, 698.46, 783.99}; // Notas principais
    int numSamples = SAMPLE_RATE * DURATION;

    int* wave = new int[numSamples]();

    // Criando a melodia alternando entre violão e piano
    int sectionSamples = numSamples / 7;
    for (int i = 0; i < 7; ++i) {
        if (i % 2 == 0) {
            generateSquareWave(wave, frequencies[i], i * sectionSamples, sectionSamples); // Violão
        } else {
            generateTriangleWave(wave, frequencies[i], i * sectionSamples, sectionSamples); // Piano
        }
    }

    // Salvar a música como arquivo WAV
    saveWav("tetris_violao_piano.wav", wave, numSamples);

    // Limpeza de memória
    delete[] wave;

    cout << "Música gerada e salva como tetris_violao_piano.wav!" << endl;
    return 0;
}

