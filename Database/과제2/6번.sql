Select customer_id,amountavg,amsum from (Select customer_id as customer_id,avg(amount) as amountavg,customer_city,sum(amount) as amsum from customer natural join depositor natural join account where (Select count(*) from
depositor where customer.customer_id=depositor.customer_id)>3 or(Select count(*) from
depositor where customer.customer_id=depositor.customer_id)=3 group by customer_id having customer_city="서울") morethanthree ;
#세개이상의 계좌를 가진 고객이 DB상에 존재하지 않아 표시되지 않음.