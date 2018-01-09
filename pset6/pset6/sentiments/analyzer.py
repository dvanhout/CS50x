import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
            
        # initialize the list
        self.pos = []
        self.neg = []
            
        # open positives file
        with open(positives, "r") as pos_file:
            
            # iterate over lines
            for line in pos_file:
                
                # eliminate comments 
                if not line.startswith(";"):
                    
                    # append to list, strip whitespace
                    self.pos.append(line.strip())
        
        # do same as above, but with negatives list     
        with open (negatives, "r") as neg_file:
            for line in neg_file:
                if not line.startswith(";"):
                    self.neg.append(line.strip())
            
        
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # initialize variable
        score = 0

        # tokenize the incoming text
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        
        # for each  in tokens list, compare against positives/negatives list
        for i in range(len(tokens)):
            
            # if word is positive, add 1
            if str.lower(tokens[i]) in self.pos:
                score += 1
            
            # if word is negative, subtract 1
            elif str.lower(tokens[i]) in self.neg:
                score -= 1
        
        return score
