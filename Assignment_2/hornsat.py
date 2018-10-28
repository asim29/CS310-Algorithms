def greedy_hornsat(variables, clauses):
    """Take a list of variables and horn clauses and return a truth assignment
    to the variables that satisfies all of them"""
    assignment = dict((v, False) for v in variables)
    print('assignment: ', assignment)
    working = set(t[1] for t in clauses if not len(t[0]))
    print('working: ', working)
    while working:
    	v = working.pop()
    	print('V is: ', v)
    	assignment[v] = True
    	vclauses = [ c for c in clauses if v in c[0]]
    	print('Relevant vclauses:',vclauses)
    	for vclause in vclauses:
    		print('removing from vclause:',vclause)
    		vclause[0].remove(v)
    		if not vclause[0]:
    			working.add(vclause[1])

    	print('While done')
    	print('')
    return assignment

def parse_file(filename):
	f = open(filename)
	line = f.readline()
	first = line.split(' ')
	num = int(first[1])

	variables = [i for i in range(1, num+1)]
	# print(variables)

	clauses = []
	negations = []	
	while line:
		line = f.readline()
		if('=>' in line):
			split1 = line.split('=>')
			left = split1[0]
			right = split1[1]
			impls = []
			# print(left)
			for c in left:
				if c.isdigit():
					impls.append(int(c))

			for c in right:
				if c.isdigit():
					val = int(c)
			# print(impls, val)

			clauses.append((impls, val))
		else:
			if(line):
				split2 = line.split('v')
				neg = []
				for i,s in enumerate(split2):
					for c in s:
						if c.isdigit():
							neg.append(int(c))
				negations.append(neg)


	# print(variables)
	# print(clauses)
	# print(negations)

	return variables, clauses, negations

def check_impls(assignment, clauses):
	for clause in clauses:
		if not all([assignment[literal] for literal in clause[0]]):
			if not assignment[clause[1]]:
				return 0

	return 1


def check_neg(assignment, negations):
	truths = []
	for i, k in assignment.items():
		if(k):
			truths.append(i)

	check = False
	for num in negations:
		if num not in truths:
			check = True

	return check

def main():
	variables, clauses, negations = parse_file('q6  test case 2.txt')
	
	print('clauses are:', clauses)
	print('negations: ', negations)
	assignment = greedy_hornsat(variables, clauses)
	if(check_impls(assignment, clauses)):
		if(check_neg(assignment, negations)):
			print('Correct assignment is:', assignment)
	else:
		print('not satisfied')

if __name__ == '__main__':
	main()