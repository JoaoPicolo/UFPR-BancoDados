select n.n_name, count(o_orderkey) from ORDERS o join CUSTOMER c on o.o_custkey = c.c_custkey join NATION n on n.n_nationkey = c.c_nationkey group by n.n_name order by count(o_orderkey) desc;