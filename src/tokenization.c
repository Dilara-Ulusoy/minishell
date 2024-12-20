#include "minishell.h"


// Bu fonksiyon pipe, logical operator vb. görüldüğünde yeni komut bekler,
// Fakat redirect operatörlerinden sonra bir arg (dosya adı) beklemelidir.
bool operator_expects_command(t_token_type type) {
    if (type == T_PIPE || type == T_AND || type == T_OR || type == T_OPEN_P || type == T_CLOSE_P || type == T_NL) {
        // Bu operatörlerden sonra komut bekler
        return true;
    } else if (type == T_REDIRECT_OUT || type == T_REDIRECT_IN || type == T_REDIRECT_APPEND || type == T_REDIRECT_HEREDOC) {
        // Bu operatörlerden sonra dosya argümanı bekler
        return false;
    }
    // Varsayılan (hiçbir duruma girmediyse) komut bekleyelim
    return true;
}

// Bu fonksiyon tokenları dolaşarak token tiplerini belirler
// Örneğin: ls | grep "hello" > output.txt
void determine_token_types(t_token *tokens, t_parser_state *state)
{
    t_token *current = tokens;
   state->expect_command = true; // Başta hep komut bekleniyor

    while (current) {
        if (current->is_operator)
        {
            // Operatör görülünce bir sonraki token için mantığı belirle
            state->expect_command = operator_expects_command(current->type);
        } else {
            // Operatör değilse yani bir word ise: örneğin ls, grep, "hello", output.txt
            if (state->expect_command) // Eger expect_command true ise word komut olmalı
            {
                current->type = T_COMMAND;
               state->expect_command = false; // Artık argüman bekleniyor cunku komut bulundu
            } else {
                // Komut değilse arg
                current->type = T_ARG;
            }
        }
        current = current->next;
    }
}

/**
 * tokenize - Bu fonksiyon, bir komut satırı girdisini alır ve boşluklara
 * göre parçalara (tokenlara) ayırır. Her token belirli bir tipe
 * (örneğin, komut, argüman, operatör) göre kategorize edilir ve daha
 * sonraki işlemler için bir bağlı listeye eklenir.
 *
 * Temel Adımlar:
 * 1. Girdi satırını boşlukları ayırıcı olarak kullanarak bir token
 *    dizisine böler.
 * 2. Her bir tokenın başlangıç tipini `get_token_type` fonksiyonu ile
 *    belirler ve `add_token` ile bağlı listeye ekler.
 * 3. Geçici olarak oluşturulan token dizisinin belleğini serbest bırakır.
 * 4. Bağlama dayalı olarak token tiplerini ayarlamak için
 *    `determine_token_types` fonksiyonunu çağırır (örneğin, bir tokenın
 *    komut mu yoksa argüman mı olduğunu belirler).
 *
 * Parametreler:
 * @line: Komut satırı girdi stringi.
 * @tokens: Tokenların depolanacağı bağlı listenin başına işaretçi.
 *
 * Döndürür:
 * - Tokenları kategorize ederek `tokens` listesine ekler.
 * - Girdi parçalanamazsa hata mesajı yazdırır ve düzgün bir şekilde çıkış yapar.
 */

void tokenize(char *line, t_token **tokens)
{
    t_parser_state parser_state;
    parser_state.expect_command = true; // Başta komut bekleniyor
    char **split_tokens;
    int i = 0;

    split_tokens = ft_split(line, ' ');
    if (!split_tokens)
    {
        printf("Error: failed to split tokens\n");
        return;
    }
    while (split_tokens[i])
    {
        t_token_type type = get_token_type(split_tokens[i]);
        add_token(tokens, split_tokens[i], type);
        i++;
    }
    i = 0;
    while (split_tokens[i]) {
        free(split_tokens[i]);
        i++;
    }
    free(split_tokens);
    determine_token_types(*tokens, &parser_state);
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
        return T_ARG; // Varsayılan olarak argüman
}


bool is_quote(char c)
{
    if(c == '"' || c == '\'')
        return true;
    return false;
}
/*

void tokenize(char *line, t_token **tokens)
{
    t_parser_state parser_state;
    parser_state.expect_command = true; // Başta komut bekleniyor
    char **split_tokens;
    int i = 0;

    split_tokens = ft_split(line, ' ');
    if (!split_tokens)
    {
        printf("Error: failed to split tokens\n");
        return;
    }

    while (split_tokens[i])
    {
        // Tırnaklı ifadeleri işleme
        if (is_quote(split_tokens[i][0]))
        {
            char quote = split_tokens[i][0]; // Açma tırnağını al
            char *merged = ft_strdup(&split_tokens[i][1]); // İlk tırnaktan sonrasını al
            int j = i + 1;

            // Tırnak kapama işareti bulunana kadar birleştir
            while (split_tokens[j] && !ft_strchr(split_tokens[j], quote))
            {
                char *temp = ft_strjoin(merged, " ");
                free(merged);
                merged = ft_strjoin(temp, split_tokens[j]);
                free(temp);
                j++;
            }

            if (split_tokens[j] && ft_strchr(split_tokens[j], quote)) // Kapanan tırnak bulundu
            {
                char *temp = ft_strjoin(merged, " ");
                free(merged);
                merged = ft_strjoin(temp, split_tokens[j]);
                free(temp);

                // Tırnakları temizle
                char *cleaned = ft_substr(merged, 0, ft_strlen(merged) - 1);
                if (parser_state.expect_command)
                {
                    add_token(tokens, cleaned, T_COMMAND);
                    parser_state.expect_command = false;
                }
                else
                {
                    add_token(tokens, cleaned, T_ARG);
                }
                free(cleaned);
                free(merged);

                i = j; // Birleştirilen tokenların sonuna atla
            }
            else
            {
                printf("Syntax error: unclosed quote\n");
                free(merged);
                break;
            }
        }
        else
        {
            // Normal tokenları işleme
            t_token_type type = get_token_type(split_tokens[i]);
            add_token(tokens, split_tokens[i], type);

            // Komut veya argüman durumunu güncelle
            if (parser_state.expect_command && type == T_COMMAND)
                parser_state.expect_command = false;
            else if (type == T_PIPE || type == T_AND || type == T_OR)
                parser_state.expect_command = true;
        }

        i++;
    }

    // split_tokens belleğini temizle
    i = 0;
    while (split_tokens[i])
    {
        free(split_tokens[i]);
        i++;
    }
    free(split_tokens);

    // Token tiplerini düzenle
    determine_token_types(*tokens, &parser_state);
}
*/