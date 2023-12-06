def part1():
    sum = 0

    with open("input.txt", "r") as file:
        for line in file:
            firstNum = ""
            lastNum = ""
            for character in line:
                if character.isdigit():
                    if firstNum == "":
                        firstNum = character
                    lastNum = character
            calibration_value = firstNum + lastNum
            sum += int(calibration_value)

    print(sum)


def part2():
    strNumbers = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"} # no zero
    strNumbersMapping = {
        "one": "1",
        "two": "2",
        "three": "3",
        "four": "4",
        "five": "5",
        "six": "6",
        "seven": "7",
        "eight": "8",
        "nine": "9"
    }
    sum = 0

    def find_first_last(string, targets):
        first_occurrence = None
        last_occurrence = None
        first_occurrence_value = None
        last_occurrence_value = None

        for i in range(len(string)):
            for target in targets:
                if string[i:i+len(target)] == target:
                    if first_occurrence is None:
                        first_occurrence = i
                        first_occurrence_value = strNumbersMapping[target]
                    last_occurrence = i
                    last_occurrence_value = strNumbersMapping[target]

        return first_occurrence, last_occurrence, first_occurrence_value, last_occurrence_value

    with open("input.txt", "r") as file:
        for line in file:
            first_occurrence, last_occurrence, first_occurrence_value, last_occurrence_value = find_first_last(line, strNumbers)
            firstNum = None
            firstNumIndex = None
            lastNum = None
            lastNumIndex = None
            for index, character in enumerate(line):
                if character.isdigit():
                    if firstNum is None:
                        firstNum = character
                        firstNumIndex = index
                    lastNum = character
                    lastNumIndex = index
            
            a = None
            b = None

            if first_occurrence is not None:
                if firstNum is not None and firstNumIndex < first_occurrence:
                    a = firstNum
                else:
                    a = first_occurrence_value
            else:
                a = firstNum

            if last_occurrence is not None:
                if lastNum is not None and lastNumIndex > last_occurrence:
                    b = lastNum
                else:
                    b = last_occurrence_value
            else:
                b = lastNum

            # no numbers found(idk if this actually happens but I don't feel like checking)
            if a is not None and b is not None:
                calibration_value = int(a + b)
                sum += calibration_value

    print(sum)


def main():
    part_selected = int(input("Which part do you want to run? Type 1 or 2: "))
    if part_selected == 1:
        part1()
    elif part_selected == 2:
        part2()

if __name__ == "__main__":
    main()
