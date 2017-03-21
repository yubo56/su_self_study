# def is_match(text, target):
#     """ naive """
#     n = len(text)
#     m = len(target)
#     for i in range(n - m + 1):
#         for j in range(m):
#             if text[i + j] != target[j]:
#                 break
#         else: # for-else means if for loop runs to completion
#             return True
#     return False


# def is_match(text, target):
#     """ rabin-karp """
#     BASE=101
#     def is_equal(str1, str2):
#         assert len(str1) == len(str2)
#         for idx in range(len(str1)):
#             if str1[idx] != str2[idx]:
#                 return False
#         return True

#     def hash_rk(substr):
#         """ rabin-karp substring hash. python no overflow problems """
#         if len(substr) == 1:
#             return ord(substr)
#         return ord(substr[-1]) + BASE * hash_rk(substr[:-1])

#     def update_hash(val, len_str, char_pop, char_push):
#         return BASE * (val - (ord(char_pop) ** len_str)) + ord(char_push)

#     n = len(text)
#     m = len(target)
#     target_hash = hash_rk(target)
#     text_hash = hash_rk(text[:m])
#     for text_idx in range(n - m):
#         if text_hash == target_hash and \
#             is_equal(target, text[text_idx: text_idx + m]):
#             return True
#         update_hash(text_hash, m, text[text_idx], text[text_idx + m])
#     return is_equal(target, text[n - m:n])


# def is_match(text, target):
#     """sub-KMP"""
#     n = len(text)
#     m = len(target)
#     i=0
#     while i < n - m + 1:
#         j=0
#         while j < m:
#             if text[i + j] != target[j]:
#                 break
#             j += 1
#         if j == m:
#             return True
#         i += j - target[:j].rfind(text[i + j])
#     return False

def is_match(text, target):
    """ hardcoded KMP for target='ababac' """
    jump_table = [
        { "a": 1 }, # if we see a, next compare target[1]
        { "b": 2, "a": 1 }, # b => next compare target[2], but if a then can next compare target[1]
        { "a": 3 },
        { "b": 4, "a": 1 },
        { "a": 5 },
        { "c": 6, "b": 4, "a": 1} # c => completion, b => 'abab' so compare target[4],
    ]
    curr_index = 0
    m = len(target)
    for c in text:
        curr_index = jump_table[curr_index].get(c, 0)
        if curr_index == m:
            return True
    return False

# print(is_match("aazaaa", "aaa"))
# print(is_match("abcbcd", "abcd"))
# print(is_match("hi my name is amelia", "amelia"))
print(is_match("abababac", "ababac"))
print(is_match("foo bar baz", "ababac"))
