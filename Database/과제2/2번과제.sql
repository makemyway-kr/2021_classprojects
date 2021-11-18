/*0번*/
use db20192698;
Alter table Customer add column customer_type varchar(10);
desc Customer;
select * from Customer;
#1번
Select customer_name from Customer where customer_city= "서울";
#2번
Select customer_name from (customer natural join borrower) join (loan_branch natural join branch) on borrower.loan_number=loan_branch.loan_number and customer.customer_city=branch.branch_city;
#3번
Select sum(loan.amount) from customer natural join borrower natural join loan where customer.customer_name like "김%";
#4번
Select branch_name,assets from branch where assets>(Select avg(assets) from branch);
#5번
Select customer.customer_id,customer.customer_name from customer where customer.customer_id not in (select customer_id from borrower) and customer.customer_id in (select
customer_id from depositor);
#6번
Select customer_id,amountavg from (Select customer_id as customer_id,avg(amount) as amountavg,customer_city from customer natural join depositor natural join account where (Select count(*) from
depositor where customer.customer_id=depositor.customer_id)>3 or(Select count(*) from
depositor where customer.customer_id=depositor.customer_id)=3 group by customer_id having customer_city="서울") morethanthree ;