select
    n.n_name,
    count(l.l_orderkey)
from
    ORDERS o
    join LINEITEM l on o.o_orderkey = l.l_orderkey
    join CUSTOMER c on o.o_custkey = c.c_custkey
    join NATION n on c.c_nationkey = n.n_nationkey
group by
    n.n_name
having
    count(l.l_orderkey) > 62000
order by
    count(l.l_orderkey) desc;