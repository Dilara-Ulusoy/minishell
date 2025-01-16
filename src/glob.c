#include "minishell.h"

/**
 * @brief match_pattern
 * Matches a string against a pattern with wildcard characters.
 * - '*' matches zero or more characters.
 * - '?' matches exactly one character.
 *
 * @param pattern The pattern to match (e.g., "*.sh").
 * @param str The string to check.
 * @return true if the string matches the pattern, false otherwise.
 */
bool match_pattern(const char *pattern, const char *str) {
    if (!*pattern && !*str)
        return true; // Both pattern and string are fully matched
    if (*pattern == '*')
        return match_pattern(pattern + 1, str) || (*str && match_pattern(pattern, str + 1));
    if (*pattern == '?' || *pattern == *str)
        return match_pattern(pattern + 1, str + 1);
    return false; // No match
}

/**
 * @brief get_cwd_entries
 * Retrieves all entries (files and directories) in the current working directory.
 *
 * @return A dynamically allocated array of strings containing entry names.
 *         Returns NULL on failure.
 */
const char **get_cwd_entries(void) {
    DIR *dir = opendir(".");
    struct dirent *entry;
    const char **entries;
    int count = 0;

    if (!dir)
        return NULL; // Failed to open directory

    while ((entry = readdir(dir)))
        count++; // Count entries
    closedir(dir);

    entries = malloc((count + 1) * sizeof(char *)); // Allocate memory for entries
    if (!entries)
        return NULL;

    dir = opendir(".");
    count = 0;
    while ((entry = readdir(dir)))
        entries[count++] = ft_strdup(entry->d_name); // Duplicate entry names
    entries[count] = NULL; // Null-terminate the array
    closedir(dir);

    return entries;
}

/**
 * @brief count_matches
 * Counts how many entries match a given pattern.
 *
 * @param entries An array of strings representing directory entries.
 * @param pattern The pattern to match against.
 * @return The number of matching entries.
 */
static size_t count_matches(const char **entries, const char *pattern) {
    size_t count = 0;
    while (*entries) {
        if (match_pattern(pattern, *entries))
            count++;
        entries++;
    }
    return count;
}

/**
 * @brief populate_matches
 * Populates an array with entries that match a given pattern.
 *
 * @param entries An array of strings representing directory entries.
 * @param pattern The pattern to match against.
 * @param matches An array to store matching entries.
 */
static void populate_matches(const char **entries, const char *pattern, const char **matches) {
    size_t index = 0;
    while (*entries) {
        if (match_pattern(pattern, *entries))
            matches[index++] = *entries; // Add matching entry to the matches array
        entries++;
    }
    matches[index] = NULL; // Null-terminate the matches array
}

/**
 * @brief glob_pattern
 * Finds and returns all entries in a directory that match a given pattern.
 *
 * @param entries An array of strings representing directory entries.
 * @param pattern The pattern to match against.
 * @return A dynamically allocated array of matching entries. Returns NULL on failure.
 */
const char **glob_pattern(const char **entries, const char *pattern) {
    size_t count = count_matches(entries, pattern); // Count matching entries
    const char **matches = malloc((count + 1) * sizeof(char *)); // Allocate memory for matches
    if (!matches) {
        perror("malloc"); // Print error if allocation fails
        return NULL;
    }
    populate_matches(entries, pattern, matches); // Populate matches array
    return matches;
}
