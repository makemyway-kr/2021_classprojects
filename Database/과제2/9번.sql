Update account set amount=case when amount>=1000 then amount*1.05 else amount*1.03 end where amount<150000;
/*15억이 상한선이고 update하면 오류가 나므로 해당 계좌는 제외*/
Select * from account;