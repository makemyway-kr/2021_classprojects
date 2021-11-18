Select * from customer natural join borrower natural join loan natural join loan_branch natural join branch;
Insert into loan values('000008',8000),('000009',10000);
Insert into borrower values ('000002','000008'),('000002','000009');
Insert into loan_branch values('000008','여의도지점');
Insert into loan_branch values ('000009','잠실지점');
/*기존 DB 1번에 만든 DB에서, 조건을 '서울의 모든 지점에' 대출 고객이 있어야 한다는 것으로 이해하여 서울의 모든지점에서 대출을 받은 고객이 존재하지 않아 이를 추가해주었음*/
Select distinct A.customer_id,A.customer_name from (Select * from customer natural join borrower natural join loan_branch natural join branch) as A WHERE not exists(
Select distinct branch_name from branch Where branch_city="서울" and branch_name NOT IN (Select distinct B.branch_name from (Select * from customer natural join borrower natural join loan_branch 
natural join branch where branch_city="서울") as B where B.customer_id=A.customer_id));