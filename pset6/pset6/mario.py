import cs50

def main():

    while True:
        # get height of pyramid from user
        print("Height: ", end="")
        height = cs50.get_int()

        # if height 0, exit
        if height == 0:
            exit(0)

        # if height in range, draw pyramid
        elif height < 0 or height > 23:
            pass

        else:
            draw_pyramid(height)
            break

def draw_pyramid(h):
    # counter
    c = 1

    # print lines
    for i in range(h):
        
        # first print leading padding (spaces) -1 each new line
        for x in range(h - c):
            print(" ", end = "")

        # then print 2 hashes
        print("##", end = "")

        # then print additional hashes... none on first line
        for y in range(c - 1):
            print("#", end = "")

        # new line
        print("")

        # increase counter
        c += 1
    
if __name__ == "__main__":
    main()