#include "minishell.h"


// Bu fonksiyon tokenları dolaşarak token tiplerini belirler
// Örneğin: ls | grep "hello" > output.txt
void determine_token_types(t_token *tokens)
{
    t_token *current = tokens;
    while (current)
    {
        if (current->is_operator)
        {
            // Tekrar kontrol etmek istiyorsanız:
            current->type = get_token_type(current->value);
        }
        else
        {
            current->type = T_WORD;
        }
        current = current->next;  // İleri gitmeyi unutmayın
    }
}

void tokenize(char *line, t_token **tokens)
{

    // split_tokens fonksiyonuyla tırnak içini koruyarak ayırıyoruz
    char **splitted = split_tokens(line);
    if (!splitted)
    {
        printf("Error: failed to split tokens\n");
        return;
    }

    // Her bir parçaya tipini belirleyip token listesine ekle
    int i = 0;
    while (splitted[i])
    {
        t_token_type type = get_token_type(splitted[i]);
        add_token(tokens, splitted[i], type);
        i++;
    }
    // heap'te oluşturulmuş stringleri free et
    i = 0;
    while (splitted[i])
    {
        free(splitted[i]);
        i++;
    }
    free(splitted);

    // Artık operator_expects_command mantığı ile
    // token tiplerini (komut mu, argüman mı vb.) yeniden güncelle
    determine_token_types(*tokens);
}

// Token tipini belirleyen fonksiyon
t_token_type get_token_type(char *token)
{
    if (ft_strncmp(token, "|", 1) == 0)
        return T_PIPE;
    else if (ft_strncmp(token, ">", 1) == 0)
        return T_REDIRECT_OUT;
    else if (ft_strncmp(token, "<", 1) == 0)
        return T_REDIRECT_IN;
    else if (ft_strncmp(token, ">>", 2) == 0)
        return T_REDIRECT_APPEND;
    else if (ft_strncmp(token, "<<", 2) == 0)
        return T_REDIRECT_HEREDOC;
    else if (ft_strncmp(token, "(", 1) == 0)
        return T_OPEN_P;
    else if (ft_strncmp(token, ")", 1) == 0)
        return T_CLOSE_P;
    else if (ft_strncmp(token, "&&", 2) == 0)
        return T_AND;
    else if (ft_strncmp(token, "||", 2) == 0)
        return T_OR;
    else if (ft_strncmp(token, "\n", 2) == 0)
        return T_NL;
    else
        return T_WORD; // Varsayılan olarak argüman
}

/**
 * Basit bir strndup fonksiyonu (42 projesinde ft_strndup ya da benzeri bir fonksiyonunuz olabilir)
 */
static char *strndup_custom(const char *src, size_t n)
{
    char *dest = (char *)malloc(n + 1);
    if (!dest)
        return NULL;
    ft_memcpy(dest, src, n);
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
    size_t max_tokens = ft_strlen(line) + 1;

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

