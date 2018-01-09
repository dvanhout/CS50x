import cs50

def main():
    # validate entry is a cent value
    while True:
        print("O hai! How much change is owed?")
        f = cs50.get_float();
        if 0 < f < 1:
           break
    
    # round up
    f *= 100

    # set counter for number of coins
    coins = 0
    
    # calculate largest coin value possible in...
    # ...25, 10, 5, 1 and subtract value from f
    coins += bite(25, f)
    f -= 25 * bite (25, f)
    
    coins += bite(10, f)
    f -= 10 * bite (10, f)

    coins += bite(5, f)
    f -= 5 * bite (5, f)
    
    coins += bite(1, f)
    f -= 1 * bite (1, f)
    
    # output number of coins to user
    print(coins)

# function calculates number of size: 'chunks' that can be removed from a value 'x' 
def bite(chunk, x):
    result = 0
    while (chunk <= x):
        x -= chunk
        result += 1
    return result
    
if __name__ == "__main__":
    main()