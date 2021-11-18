Select customer.customer_id,customer.customer_name from customer where customer.customer_id not in (select customer_id from borrower) and customer.customer_id in (select
customer_id from depositor);