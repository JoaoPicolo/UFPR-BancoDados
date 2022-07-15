select
    n.n_name,
    sum(l.l_quantity)
from
    CUSTOMER c
    left join ORDERS o on c.c_custkey = o.o_custkey
    join LINEITEM l on l.l_orderkey = o.o_orderkey
    left join nation n on n.n_nationkey = c.c_nationkey
group by
    n.n_name
order by
    sum(l.l_quantity) desc
limit
    10;