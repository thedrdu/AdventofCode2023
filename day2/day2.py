def part1():
    sum = 0
    
    max_colors = {
        "red": 12,
        "green": 13,
        "blue": 14
    }

    with open("input.txt", "r") as file:
        for line in file:
            line_a, line_b = line.split(":")
            game_id = int(line_a[5:])
            bags = line_b.split(";")
            valid = True
            for bag in bags:
                colors = bag.split(",")
                color_counts = {}
                for color in colors:
                    parts = color.strip().split(' ')
                    count = int(parts[0])
                    color = parts[1]
                    color_counts[color] = count
                for color in color_counts:
                    if color_counts[color] > max_colors[color]:
                        valid = False
                        break
            if valid:
                sum += game_id
    print(sum)

def part2():
    sum = 0

    with open("input.txt", "r") as file:
        for line in file:
            min_max_colors = {
                "red": -1,
                "blue": -1,
                "green": -1
            }
            line_a, line_b = line.split(":")
            bags = line_b.split(";")
            for bag in bags:
                colors = bag.split(",")
                color_counts = {}
                for color in colors:
                    parts = color.strip().split(' ')
                    count = int(parts[0])
                    color = parts[1]
                    color_counts[color] = count
                for color in color_counts:
                    if color_counts[color] > min_max_colors[color]:
                        min_max_colors[color] = color_counts[color]
            sum += min_max_colors["red"] * min_max_colors["blue"] * min_max_colors["green"]
    print(sum)


def main():
    part_selected = int(input("Which part do you want to run? Type 1 or 2: "))
    if part_selected == 1:
        part1()
    elif part_selected == 2:
        part2()

if __name__ == "__main__":
    main()
