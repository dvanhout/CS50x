import cs50, sys

def main():
    # check for proper usage of command line arguments
    if len(sys.argv) != 2:
        print("Usage: {} <key>".format(sys.argv[0]))
        exit(1)

    # store argv[1] as integer into key variable
    key = int(sys.argv[1])
    
    # prompt for a string
    print("plaintext: ", end = "")
    s = cs50.get_string()

    # shift characters ahead over by key 
    print("ciphertext: ", end = "")
    for i in range(len(s)):
        if str.isalpha(s[i]):
            if str.islower(s[i]):
                a = ord('a') + ((ord(s[i]) - ord('a') + key) % 26)
                print(chr(a), end = "")
            elif str.isupper(s[i]):
                a = ord('A') + ((ord(s[i]) - ord('A') + key) % 26)
                print(chr(a), end = "")
        else:
            print(s[i], end = "")
    print("")
    
if __name__ == "__main__":
    main()