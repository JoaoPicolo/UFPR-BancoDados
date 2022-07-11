select
    c_mktsegment,
    count(c_custkey)
from
    CUSTOMER
group by
    c_mktsegment;