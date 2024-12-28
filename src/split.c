#include "minishell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Basit bir strndup fonksiyonu (42 projesinde ft_strndup ya da benzeri bir fonksiyonunuz olabilir)
 */
static char *strndup_custom(const char *src, size_t n)
{
    char *dest = (char *)malloc(n + 1);
    if (!dest)
        return NULL;
    memcpy(dest, src, n);
    dest[n] = '\0';
    return dest;
}

/**
 * split_tokens:
 *   Verilen 'line' içinde tırnakları (hem tek ' hem çift ") dikkate alarak
 *   tokenlara böler. Tırnak içindeki boşluklar ayrılmadan aynı token içinde kalır.
 *
 * Örnek:
 *   girdi:  echo "merhaba dunya" 'tek tırnaklı  arg' arg3
 *   çıktı:  [ "echo", "merhaba dunya", "tek tırnaklı  arg", "arg3", NULL ]
 */
char **split_tokens(const char *line)
{
    // Kaç token bulabileceğimizi kestirmek için en kötü ihtimalle
    // her karakter bir token olsa bile +1 adet son NULL için yer ayıralım:
    size_t max_tokens = strlen(line) + 1;

    // Çıktı (token) dizisini oluşturuyoruz.
    // Sonunda NULL ile işaretlenecek.
    char **tokens = (char **)malloc(sizeof(char *) * max_tokens);
    if (!tokens)
        return NULL;

    size_t token_count = 0;        // Kaç adet token bulduğumuzu takip edecek
    bool   in_single_quote = false;
    bool   in_double_quote = false;
    size_t i = 0;   // line üzerinde gezer
    size_t start = 0; // token başlangıcı

    while (line[i] != '\0')
    {
        char c = line[i];

        // Eğer tek tırnak açılmamışsa ve çift tırnak karakteri görürsek
        if (c == '\"' && !in_single_quote)
        {
            // Mevcut durumda çift tırnağı kapatıyoruz veya açıyoruz
            in_double_quote = !in_double_quote;
        }
        // Eğer çift tırnak açılmamışsa ve tek tırnak karakteri görürsek
        else if (c == '\'' && !in_double_quote)
        {
            // Mevcut durumda tek tırnağı kapatıyoruz veya açıyoruz
            in_single_quote = !in_single_quote;
        }
        // Tırnak dışında isek ve boşluk görürsek token sonunu işaretler
        else if ((c == ' ' || c == '\t') && !in_single_quote && !in_double_quote)
        {
            // Mevcut token var mı? (start < i ise var)
            if (i > start)
            {
                tokens[token_count++] = strndup_custom(line + start, i - start);
            }
            // Boşluktan sonraki konumdan yeni token başlayacak
            start = i + 1;
        }
        i++;
    }

    // Döngü bittiğinde (line[i] == '\0') ama hala start < i olabilir.
    // Bu da son bir token'ın buffer'da kalmış olabileceği anlamına geliyor.
    if (i > start)
    {
        tokens[token_count++] = strndup_custom(line + start, i - start);
    }

    // Son eleman olarak NULL atayalım.
    tokens[token_count] = NULL;

    return tokens;
}

/**
 * Aşağıda sadece testleri gerçekleştiren main fonksiyonunu yazıyoruz.
 * Derleyip çalıştırdığınızda, girdi satırlarının nasıl tokenize edildiğini
 * görebilirsiniz.
 */
int main(void)
{
    // Test için farklı input senaryoları
    const char *test_inputs[] = {
        "echo \"merhaba dunya\" 'tek tırnaklı  arg' arg3",
        "ls -l -a \"dosya adi.txt\"",
        "'tek tirnak'\"hemen sonra cift\"",
        "  bosluklarla   baslayan  string  ",
        "echo 'iç içe \" (ve) ' \"karmaşık '  ",
        NULL
    };

    int test_index = 0;
    while (test_inputs[test_index])
    {
        printf("Input  : [%s]\n", test_inputs[test_index]);
        char **result = split_tokens(test_inputs[test_index]);
        if (!result)
        {
            printf("split_tokens returned NULL!\n");
        }
        else
        {
            // Tokenleri yazdıralım
            int i = 0;
            while (result[i])
            {
                printf("  Token[%d]: [%s]\n", i, result[i]);
                free(result[i]); // Kullanımdan sonra free etmeyi unutmayın
                i++;
            }
            free(result); // Dizi pointer'ını da free'leyelim
        }
        printf("--------------------------------------------------\n");
        test_index++;
    }

    return 0;
}
