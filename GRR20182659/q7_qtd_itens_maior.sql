select
    n.n_name,
    sum(l.l_quantity)
from
    ORDERS o
    join LINEITEM l on o.o_orderkey = l.l_orderkey
    join CUSTOMER c on o.o_custkey = c.c_custkey
    join NATION n on c.c_nationkey = n.n_nationkey
group by
    n.n_name
having
    sum(l.l_quantity) > 62000
order by
    sum(l.l_quantity) desc;