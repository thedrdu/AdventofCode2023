package day5;

import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;

public class day5{
    private static void part1(){
        long p1_result = Long.MAX_VALUE; // assuming all input values are within the range of a long...

        File file = new File("input.txt");
        Scanner scanner;
        try{
            scanner = new Scanner(file);
        }
        catch(FileNotFoundException e){
            e.printStackTrace();
            return;
        }

        String line = scanner.nextLine(); // first line with seed data
        String seedData = line.substring(line.indexOf(":") + 2); // seedStart at the values we actually care about
        Scanner subscanner = new Scanner(seedData); // used to separate the values inside a line
        
        ArrayList<Long> seeds = new ArrayList<Long>();
        while(subscanner.hasNextLong()){ // really cool how Java has this method for input scanning
            seeds.add(subscanner.nextLong());
        }
        subscanner.close();

        scanner.nextLine(); // move to the empty line so we can align in future scanning

        while(scanner.hasNextLine()){
            scanner.nextLine(); // skip the map line with no actual data

            // we have to clone seeds since we can't update it as we go along, because
            // we need to preserve the starting values of seeds at each new mapping section
            // (we need to base on the original seed values when checking ranges)
            ArrayList<Long> newVals = new ArrayList<Long>();
            for(long i : seeds){
                newVals.add(i);
            }

            while(scanner.hasNextLine()){ // we go through all the ranges in the current mapping
                line = scanner.nextLine();
                if(line.length() == 0){
                    break;
                }

                subscanner = new Scanner(line);
                long[] lineInfo = new long[3];
                for(int i = 0; i < 3; i++){
                    lineInfo[i] = subscanner.nextLong(); // read in the line's 3 values
                }

                subscanner.close();

                long destinationStart = lineInfo[0], sourceStart = lineInfo[1], range = lineInfo[2];

                for(int i = 0; i < seeds.size(); i++){
                    long currentSeed = seeds.get(i);
                    
                    if(currentSeed >= sourceStart && currentSeed < sourceStart + range){ // current seed is in the range of our current line info
                        newVals.set(i, currentSeed + destinationStart - sourceStart); // current seed value is shifted according to the line info
                    }
                }
            }

            seeds = newVals; // save the shifts we made to the seed values(if any)
        }

        scanner.close();

        for(long i : seeds){
            p1_result = Long.min(i, p1_result);
        }

        System.out.println("Seed with lowest location: " + p1_result);
    }

    private static void part2(){
        long p2_result = Long.MAX_VALUE; // assuming all input values are within the range of a long...

        File file = new File("input.txt");
        Scanner scanner;
        try{
            scanner = new Scanner(file);
        }
        catch(FileNotFoundException e){
            e.printStackTrace();
            return;
        }

        String line = scanner.nextLine();
        String seedData = line.substring(line.indexOf(":") + 2);
        Scanner subscanner = new Scanner(seedData);
        
        ArrayList<long[]> seedRanges = new ArrayList<long[]>(); // we can't just store the seeds directly, too many of them

        while(subscanner.hasNextLong()){
            long seed = subscanner.nextLong(), range = subscanner.nextLong();
            long[] insertRange = new long[] {seed, seed + range - 1};
            seedRanges.add(insertRange);
        }
        subscanner.close();

        scanner.nextLine();

        while(scanner.hasNextLine()){
            scanner.nextLine(); // skip the map line with no actual data

            ArrayList<long[]> mappedRanges = new ArrayList<long[]>();

            while(scanner.hasNextLine()){
                line = scanner.nextLine();
                if(line.length() == 0){
                    break;
                }

                subscanner = new Scanner(line);
                long[] lineInfo = new long[3];
                for(int i = 0; i < 3; i++){
                    lineInfo[i] = subscanner.nextLong();
                }

                subscanner.close();

                long destinationStart = lineInfo[0], sourceStart = lineInfo[1], sourceRange = lineInfo[2]; // distinguish this range from seed range

                for(int i = 0; i < seedRanges.size(); i++){
                    long seedStart = seedRanges.get(i)[0], seedRange = seedRanges.get(i)[1];

                    if(seedStart < sourceStart + sourceRange && seedRange >= sourceStart){ // ranges overlap
                        long[] shiftedSeedRange = {seedStart + destinationStart - sourceStart, seedRange + destinationStart - sourceStart};
                        boolean hasUnmapped = false; // if we have unmapped areas(ex. from moving a seed's range)

                        if(seedStart < sourceStart){ // if the seed starts before the line's starting point, shrink the seed's range
                            shiftedSeedRange[0] = destinationStart;
                            seedRanges.get(i)[1] = sourceStart - 1;
                            hasUnmapped = true;
                        }
                        if(seedRange >= sourceStart + sourceRange){ // if the seed starts after the line's source+range, increase the seed's starting point
                            shiftedSeedRange[1] = destinationStart + sourceRange - 1;
                            if(hasUnmapped){ // we might need to cover a new gap
                                seedRanges.add(new long[] {sourceStart + sourceRange, seedRange});
                            }
                            else{
                                seedRanges.get(i)[0] = sourceStart + sourceRange;
                                hasUnmapped = true;
                            }
                        }
                        mappedRanges.add(shiftedSeedRange);
                        if(!hasUnmapped){
                            seedRanges.remove(i--); //the current range does nothing for us(none of the if statements were triggered)
                        }
                    }
                }
            }

            seedRanges.addAll(mappedRanges); // save the shifts we made to the seed values(if any)
        }

        scanner.close();

        for(long[] seedRange : seedRanges){
            p2_result = Long.min(p2_result, seedRange[0]);
        }

        System.out.println("Seed with lowest location: " + p2_result);
    }


    public static void main(String[] args){
        if(args.length != 1){
            System.out.println("Please provide 1 argument (either \"1\" or \"2\")");
            return;
        }
        if(args[0].equalsIgnoreCase("1")){
            part1();
        }
        else if(args[0].equalsIgnoreCase("2")){
            part2();
        }
        else{
            System.out.println("Please provide 1 argument (either \"1\" or \"2\")");
            return;
        }
        return;
    }
}