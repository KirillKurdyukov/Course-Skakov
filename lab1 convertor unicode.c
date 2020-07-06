#include <stdio.h>
#include <string.h>
// Автор проекта грешный Кирилл


void reverseForUTF16(char *res, int size) {
    if (size == 2) {
        char temp1;
        temp1 = res[0];
        res[0] = res[1];
        res[1] = temp1;
    }
    if (size == 4) {
        char temp1, temp2;
        temp1 = res[0];
        temp2 = res[2];
        res[0] = res[1];
        res[1] = temp1;
        res[2] = res[3];
        res[3] = temp2;
    }
}

void UTF8convertUTF8BOM(FILE *in, FILE *out) {
    char ch;
    fprintf(out, "%c%c%c", 0xEF, 0xBB, 0xBF);
    while(fscanf(in, "%c", &ch) != EOF)
        fprintf(out, "%c", ch);
}

void UTF8BOMconvertUTF8(FILE *in, FILE *out) {
    char ch, a, b, c;
    fscanf(in, "%c%c%c", &a, &b, &c);
    while(fscanf(in, "%c", &ch) != EOF)
        fprintf(out, "%c", ch);
}

void intoMyself(FILE *in, FILE *out) {
    unsigned char ch;
    while(1) {
        fread(&ch, sizeof(ch), 1, in);
        if (feof(in))
            break;
        fwrite(&ch, sizeof(ch), 1, out);
    }
}

// Википедия моя радость
unsigned long giveCPUTF16(const unsigned char * byte, int NumberOfBytes) {
    switch (NumberOfBytes) {
        case 2:
            return (byte[0] << 8) + byte[1];
        case 4:
            return ((((byte[0] << 8) + byte[1]) - 0xD800) * 0x0400) + (((byte[2] << 8) + byte[3]) - 0xDC00) + 0x10000;
    }
}

unsigned long giveCPUTF32(const unsigned char * byte) {
    return (byte[0] << 24) + (byte[1] << 16) + (byte[2] << 8) + (byte[3]);
}

void reverse(char *res, int size) {
    if (size == 2) {
        char temp1;
        temp1 = res[0];
        res[0] = res[1];
        res[1] = temp1;
    }
    if (size == 4) {
        char temp1, temp2;
        temp1 = res[0];
        temp2 = res[1];
        res[0] = res[3];
        res[1] = res[2];
        res[3] = temp1;
        res[2] = temp2;
    }
}
unsigned long giveCPUTF8(const unsigned char * byte, int NumberOfBytes) {
    switch (NumberOfBytes) {
        case 1:
            return byte[0];
        case 2:
            return (byte[0] - (1 << 6) - (1 << 7))
                   * (1 << 6) + (byte[1] - (1 << 7));
        case 3:
            return (byte[0] - (1 << 6) - (1 << 7) - (1 << 5))
                   * (1 << 12) + (byte[1] - (1 << 7)) * (1 << 6) + (byte[2] - (1 << 7));
        case 4:
            return (byte[0] - (1 << 6) - (1 << 7) - (1 << 5) - (1 << 4))
                   * (1 << 18) + (byte[1] - (1 << 7)) * (1 << 12) + (byte[2] - (1 << 7)) * (1 << 6) + (byte[3] - (1 << 7));
    }
}

void codePointConvertUTF32(unsigned char* res, const unsigned long codePoint) {
    res[0] = codePoint >> 24;
    res[1] = (codePoint << 8) >> 24;
    res[2] = (codePoint << 16) >> 24;
    res[3] = (codePoint << 24) >> 24;
}

int codePointConvertUTF16(unsigned char* res, unsigned long codePoint) {
    if (codePoint <= 0xFFFF) {
        res[0] = codePoint >> 8;
        res[1] = codePoint;
        return 2;
    } else if (codePoint <= 0x10FFFF) {
        codePoint = codePoint - 0x10000;
        res[0] = ((codePoint >> 10) + 0xD800) >> 8;;
        res[1] = (((codePoint >> 10) + 0xD80) << 24) >> 24;
        res[2] = (((codePoint << 22) >> 22) + 0xDC00) >> 8;;
        res[3] = ((((codePoint << 22) >> 22) + 0xDC00) << 24) >> 24;
        return 4;
    }
    return 0;
}

int codePointConvertUTF8(unsigned char* res, unsigned long codePoint) {
    if (codePoint <= 0x007F) {
        res[0] = codePoint;
        return 1;
    }
    if (codePoint <= 0x07FF) {
        res[0] = codePoint / 64 + 0b11000000;
        res[1] = codePoint % 64 + 0b11000000;
        return 2;
    }
    if (codePoint <= 0xFFFF) {
        res[0] = codePoint / (1 << 12) + 0b11100000;
        res[1] = codePoint % (1 << 12) / (1 << 6) + 0b10000000;
        res[2] = codePoint % (1 << 6) + 0b10000000;
        return 3;
    }
    if ((codePoint <= 0x10FFFF)) {
        res[0] = codePoint / (1 << 18) + 0b11110000;
        res[1] = codePoint % (1 << 18) / (1 << 12) + 0b10000000;
        res[2] = codePoint % (1 << 12) / (1 << 6) + 0b10000000;
        res[3] = codePoint % (1 << 6) + 0b10000000;
        return 4;
    }
    printf("ERROR");
    return -1;
}

void UTF8ConvertUTF32(FILE *in, FILE *out, _Bool endian /* endian = 0 => BE, else LE */) {
    unsigned char byte[4];
    unsigned char res[4];
    if (endian)
        fprintf(out,"%c%c%c%c", 0xFF, 0xFE, 0x00, 0x00);
    else
        fprintf(out,"%c%c%c%c", 0x00, 0x00, 0xFE, 0xFF);
    while(fscanf(in, "%c", &byte[0]) != EOF) {
        if (byte[0] <= 0x007F) {
            codePointConvertUTF32(res, giveCPUTF8(byte, 1));
            if (endian) {
                reverse(res,4 );
            }
            fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        }
        if(fscanf(in, "%c", &byte[1]) == EOF)
            break;
        unsigned int cp;
        if ((cp = giveCPUTF8(byte, 2)) <= 0x07FF) {
            codePointConvertUTF32(res, cp);
            if (endian) {
                reverse(res, 4);
            }
            fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        }
        if(fscanf(in, "%c", &byte[2]) == EOF)
            break;
        if ((cp = giveCPUTF8(byte, 3)) <= 0xFFFF) {
            codePointConvertUTF32(res, cp);
            if (endian) {
                reverse(res, 4);
            }
            fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        }
        if(fscanf(in, "%c", &byte[3]) == EOF)
            break;
        if ((cp = giveCPUTF8(byte, 4)) <= 0x10FFFF) {
            codePointConvertUTF32(res, cp);
            if (endian) {
                reverse(res, 4);
            }
            fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        } else {
            fprintf(out, "ERROR");//?
        }
    }
}

void UTF8ConvertUTF16(FILE *in, FILE *out, _Bool endian /* endian = 0 => BE, else LE */) {
    unsigned char byte[4];
    unsigned char res[4];
    if (endian)
        fprintf(out,"%c%c", 0xFF, 0xFE);
    else
        fprintf(out,"%c%c", 0xFE, 0xFF);
    while(fscanf(in, "%c", &byte[0]) != EOF) {
        if (byte[0] <= 0x007F) {
            int amountByte = codePointConvertUTF16(res, giveCPUTF8(byte, 1));
            if (endian) {
                reverse(res, amountByte);
            }
            fprintf(out, "%c%c", res[0], res[1]);
            continue;
        }
        if(fscanf(in, "%c", &byte[1]) == EOF)
            break;
        unsigned int cp;
        if ((cp = giveCPUTF8(byte, 2)) <= 0x07FF) {
            int amountByte = codePointConvertUTF16(res, cp);
            if (endian) {
                reverseForUTF16(res, amountByte);
            }
            fprintf(out, "%c%c", res[0], res[1]);
            continue;
        }
        if(fscanf(in, "%c", &byte[2]) == EOF)
            break;
        if ((cp = giveCPUTF8(byte, 3)) <= 0xFFFF) {
            int amountByte = codePointConvertUTF16(res, cp);
            if (endian) {
                reverseForUTF16(res, amountByte);
            }
            if (amountByte == 2)
                fprintf(out, "%c%c", res[0], res[1]);
            else
                fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        }
        if(fscanf(in, "%c", &byte[3]) == EOF)
            break;
        if ((cp = giveCPUTF8(byte, 4)) <= 0x10FFFF) {
            int amountByte = codePointConvertUTF16(res, cp);
            if (endian) {
                reverseForUTF16(res, amountByte);
            }
            if (amountByte == 2)
                fprintf(out, "%c%c", res[0], res[1]);
            else
                fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
            continue;
        } else {
            fprintf(out, "ERROR");//?
        }
    }
}

void UTF32ConvertUTF8(FILE *in, FILE *out, _Bool haveBom, _Bool endian /* endian = 0 => BE, else LE */) {
    unsigned char byte[4];
    fscanf(in, "%c%c%c%c", &byte[0], &byte[1], &byte[2], &byte[3]);
    unsigned char res[4];
    if (haveBom)
        fprintf(out,"%c%c%c", 0xEF, 0xBB, 0xBF);
    while(fscanf(in, "%c%c%c%c", &byte[0], &byte[1], &byte[2], &byte[3]) != EOF) {
        if (endian) {
            reverse(byte, 4);
        }
        switch (codePointConvertUTF8(res, giveCPUTF32(byte))) {
            case 1:
                fprintf(out, "%c", res[0]);
                break;
            case 2:
                fprintf(out, "%c%c", res[0], res[1]);
                break;
            case 3:
                fprintf(out, "%c%c%c", res[0], res[1], res[2]);
                break;
            case 4:
                fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
                break;
        }
    }
}

void UTF16ConvertUTF8(FILE *in, FILE *out, _Bool haveBom, _Bool endian /* endian = 0 => BE, else LE */) {
    unsigned char byte[4];
    fscanf(in, "%c%c", &byte[0], &byte[1]);
    unsigned char res[4];
    if (haveBom)
        fprintf(out,"%c%c%c", 0xEF, 0xBB, 0xBF);
    while (fscanf(in, "%c%c", &byte[0], &byte[1]) != EOF) {
        if (endian) {
            reverseForUTF16(byte, 2);
        }
        unsigned long cp;
        if ((cp = giveCPUTF16(byte, 2)) <= 0xD7FF) {
            switch (codePointConvertUTF8(res, cp)) {
                case 1:
                    fprintf(out, "%c", res[0]);
                    break;
                case 2:
                    fprintf(out, "%c%c", res[0], res[1]);
                    break;
                case 3:
                    fprintf(out, "%c%c%c", res[0], res[1], res[2]);
                    break;
                case 4:
                    fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
                    break;
            }
            continue;
        }
        if (endian) {
            reverseForUTF16(byte, 2);
        }
        if (fscanf(in, "%c%c", &byte[2], &byte[3]) == EOF)
            break;
        //int v = fscanf(in, "%c%c", &byte[2], &byte[3]);
        if (endian) {
            reverseForUTF16(byte, 4);
        }
        if ((cp = giveCPUTF16(byte, 4)) <= 0x10FFFF) {
            switch (codePointConvertUTF8(res, cp)) {
                case 1:
                    fprintf(out, "%c", res[0]);
                    break;
                case 2:
                    fprintf(out, "%c%c", res[0], res[1]);
                    break;
                case 3:
                    fprintf(out, "%c%c%c", res[0], res[1], res[2]);
                    break;
                case 4:
                    fprintf(out, "%c%c%c%c", res[0], res[1], res[2], res[3]);
                    break;
            }
            continue;
        } else printf("kek");
    }
}


int main(int argc, char **argv) {
    //unsigned int numberUTF;
    if (argc != 4) {
        printf("There was no unexpected error, please try again");
        return 0;
    }
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        printf("Not found input file exception");
        fclose(in);
        return 0;
    }
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        printf("Not found output file exception");
        fclose(out);
        return 0;
    }
    FILE *tired = fopen("tired.txt", "w+");
    if (!tired) {
        printf("Not found output file exception");
        fclose(out);
        return 0;
    }
    unsigned char BOM[4];
    fread(BOM, sizeof(char), 4, in);
    if (memcmp(BOM, "\x00\x00\xFE\xFF", 4) == 0) {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                UTF32ConvertUTF8(in, out, 0, 0);
                fclose(in);
                break;
            case '1':
                UTF32ConvertUTF8(in, out, 1, 0);
                fclose(in);
                break;
            case '2':
                UTF32ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 1);
                fclose(in);
                break;
            case '3':
                UTF32ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 0);
                fclose(in);
                break;
            case '4':
                UTF32ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 1);
                fclose(in);
                break;
            case '5':
                intoMyself(in, out);
                break;
        }
    } else if (memcmp(BOM, "\xFF\xFE\x00\x00", 4) == 0) {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                UTF32ConvertUTF8(in, out, 0, 1);
                fclose(in);
                break;
            case '1':
                UTF32ConvertUTF8(in, out, 1, 1);
                fclose(in);
                break;
            case '2':
                UTF32ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 1);
                fclose(in);
                break;
            case '3':
                UTF32ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 0);
                fclose(in);
                break;
            case '4':
                intoMyself(in, out);
                fclose(in);
                break;
            case '5':
                UTF32ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 0);
                fclose(in);
                break;
        }
    } else if (memcmp(BOM, "\xFE\xFF", 2) == 0) {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                UTF16ConvertUTF8(in, out, 0, 0);
                fclose(in);
                break;
            case '1':
                UTF16ConvertUTF8(in, out, 1, 0);
                fclose(in);
                break;
            case '2':
                UTF16ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 1);
                fclose(in);
                break;
            case '3':
                intoMyself(in, out);
                break;
            case '4':
                UTF16ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 1);
                fclose(in);
                break;
            case '5':
                UTF16ConvertUTF8(in, tired, 0, 0);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 0);
                fclose(in);
                break;
        }
    } else if (memcmp(BOM, "\xFF\xFE", 2) == 0) {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                UTF16ConvertUTF8(in, out, 0, 1);
                fclose(in);
                break;
            case '1':
                UTF16ConvertUTF8(in, out, 1, 1);
                fclose(in);
                break;
            case '2':
                intoMyself(in, out);
                fclose(in);
                break;
            case '3':
                UTF16ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 0);
                fclose(in);
                break;
            case '4':
                UTF16ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 1);
                fclose(in);
                break;
            case '5':
                UTF16ConvertUTF8(in, tired, 0, 1);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 0);
                fclose(in);
                break;
        }
    } else if (memcmp(BOM, "\xEF\xBB\xBF", 3) == 0) {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                UTF8BOMconvertUTF8(in, out);
                fclose(in);
                break;
            case '1':
                intoMyself(in, out);
                fclose(in);
                break;
            case '2':
                UTF8BOMconvertUTF8(in, tired);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 1);
                fclose(in);
                break;
            case '3':
                UTF8BOMconvertUTF8(in, tired);
                rewind(tired);
                UTF8ConvertUTF16(tired, out, 0);
                fclose(in);
                break;
            case '4':
                UTF8BOMconvertUTF8(in, tired);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 1);
                fclose(in);
                break;
            case '5':
                UTF8BOMconvertUTF8(in, tired);
                rewind(tired);
                UTF8ConvertUTF32(tired, out, 0);
                fclose(in);
                break;
        }
    } else {
        fclose(in);
        FILE *in = fopen(argv[1], "r");
        if (!in) {
            printf("Not found input file exception");
            fclose(in);
            return 0;
        }
        switch (argv[3][0]) {
            case '0':
                intoMyself(in, out);
                fclose(in);
                break;
            case '1':
                UTF8convertUTF8BOM(in, out);
                fclose(in);
                break;
            case '2':
                UTF8ConvertUTF16(in, out, 1);
                fclose(in);
                break;
            case '3':
                UTF8ConvertUTF16(in, out, 0);
                fclose(in);
                break;
            case '4':
                UTF8ConvertUTF32(in, out, 1);
                fclose(in);
                break;
            case '5':
                UTF8ConvertUTF32(in, out, 0);
                fclose(in);
                break;
        }
    }
    fclose(out);
    fclose(tired);
    fclose(in);
    return 0;
}
// автор умер