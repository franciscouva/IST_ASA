from pulp import LpProblem, LpMaximize, lpSum, value, LpVariable, LpInteger, GLPK

# Input: Number of toys, number of packages, and maximum production
num_toys, num_packages, max_production = map(int, input().split())

# Input: Toys information (profit and capacity)
toys = {i + 1: {'profit': int(profit), 'capacity': int(capacity)}
        for i, (profit, capacity) in enumerate([input().split() for _ in range(num_toys)])}

# Input: Packages information (products, profit, and number of products)
packages = {i + 1: {'products': set(map(int, products[:-1])), 'profit': int(products[-1]), 'num_products': len(products) - 1}
            for i, products in enumerate([input().split() for _ in range(num_packages)])}

# Create a linear programming problem
prob = LpProblem("Maximize_Profit", LpMaximize)

# Define variables for toys and packages
toy_vars = [LpVariable(f"Toy_{i}", 0, toy['capacity'], LpInteger) for i, toy in toys.items()]
package_vars = [LpVariable(f"Package_{i}", 0, min(toys[product]['capacity'] for product in package['products']), LpInteger)
                for i, package in packages.items()]

# Constraints: Each toy's production capacity and package constraints
for i, toy_var in enumerate(toy_vars, start=1):
    prob += toy_var + lpSum(package_vars[j-1] for j, package in packages.items() if i in package['products']) <= toys[i]['capacity']

# Constraint: Total production should not exceed the maximum allowed production
prob += lpSum(toy_vars) + lpSum(package_vars[j-1] * packages[j]['num_products'] for j in packages) <= max_production

# Objective function: Maximize the total profit
prob += lpSum(toys[i]['profit'] * toy_var for i, toy_var in enumerate(toy_vars, start=1)) + lpSum(packages[j]['profit'] * package_var for j, package_var in enumerate(package_vars, start=1))

# Solve the linear programming problem using GLPK solver
prob.solve(GLPK(msg=0))

# Print the maximum daily profit
print(int(value(prob.objective)))
