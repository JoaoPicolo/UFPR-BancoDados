select
    count(c_custkey)
from
    (
        select
            c.c_custkey
        from
            CUSTOMER c
            join ORDERS o on c.c_custkey = o.o_custkey
        where
            o.o_comment like '%special request%'
        except
        select
            c.c_custkey
        from
            CUSTOMER c
            join ORDERS o on c.c_custkey = o.o_custkey
        where
            o.o_comment like '%unusual package%'
    );