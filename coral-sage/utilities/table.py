#!/usr/bin/env python3


def jointable(table1, table2, join="|"):
    # Find first line that contains `join`
    index1 = [num for num, row in enumerate(table1) if join in "".join([str(_) for _ in row])][0]
    positions1 = [i for i, elt in enumerate(table1[index1]) if elt == join] + [len(table1[index1])]
    index2 = [num for num, row in enumerate(table2) if join in "".join([str(_) for _ in row])][0]
    positions2 = [i for i, elt in enumerate(table2[index2]) if elt == join] + [len(table2[index2])]

    assert len(positions1) == len(positions2)

    table = []
    for row1 in table1:
        row = []
        row += [""] * (positions2[0] - positions1[0]) + row1[: positions1[0] + 1]
        for n_position in range(1, len(positions1)):
            diff = (positions2[n_position] - positions2[n_position - 1]) - (
                positions1[n_position] - positions1[n_position - 1]
            )
            row += [""] * diff + row1[positions1[n_position - 1] + 1 : positions1[n_position] + 1]
        table += [row]

    table += [["" for _ in range(len(table[0]))]]

    for row2 in table2:
        row = []
        row += [""] * (positions1[0] - positions2[0]) + row2[: positions2[0] + 1]
        for n_position in range(1, len(positions2)):
            diff = (positions1[n_position] - positions1[n_position - 1]) - (
                positions2[n_position] - positions2[n_position - 1]
            )
            row += [""] * diff + row2[positions2[n_position - 1] + 1 : positions2[n_position] + 1]
        table += [row]

    return table


def strtable(table, columns=None, widths=None, subcolumns=None, sep="  ", div=None):
    if subcolumns is not None:
        table.insert(0, subcolumns)

    if columns is not None:
        table.insert(0, columns)

    max_row_length = max([len(row) for row in table])
    for n, row in enumerate(table):
        if len(row) < max_row_length:
            print(":: Warning adjusting table size")
            table[n] = row + [""] * (max_row_length - len(row))

    if div is not None:
        divider = ["" for _ in range(len(table[0]))]
        for position in div:
            table.insert(position, divider)

    if widths is None:
        widths = [max([len(row[i]) for row in table]) for i in range(len(table[0]))]

    return "\n".join([sep.join([f"{item:>{widths[i]}}" for i, item in enumerate(row)]) for row in table])


def printtable(table, columns=None, sep=" ", div=None, border="="):
    table_str = strtable(table, columns=columns, sep=sep, div=div)
    if border is not None:
        print(border * len(table_str.split("\n")[0]))
    print(table_str)
    if border is not None:
        print(border * len(table_str.split("\n")[0]))
