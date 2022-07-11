select
    n.n_name,
    count(l.l_orderkey)
from
    CUSTOMER c
    left join ORDERS o on c.c_custkey = o.o_custkey
    join LINEITEM l on l.l_orderkey = o.o_orderkey
    left join nation n on n.n_nationkey = c.c_nationkey
group by
    n.n_name
order by
    count(l.l_orderkey) desc
limit
    10;