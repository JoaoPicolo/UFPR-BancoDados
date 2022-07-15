select
    n.n_name,
    count(o.o_orderkey)
from
    CUSTOMER c
    left join ORDERS o on c.c_custkey = o.o_custkey
    left join NATION n on c.c_nationkey = n.n_nationkey
group by
    n.n_name
order by
    count(o.o_orderkey) desc;