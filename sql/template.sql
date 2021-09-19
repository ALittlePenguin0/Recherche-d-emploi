USE [DSTRAINING]
GO
/****** Object:  StoredProcedure [dbo].[__tmpl__BLD_WRK_TableName]    Script Date: 19/09/2021 22:14:04 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		
-- Create date: 
-- Description:	RAW -> WRK
-- MOD DATE: 
-- =============================================
ALTER PROC [dbo].[__tmpl__BLD_WRK_TableName]


AS
BEGIN

-- =============================================
-- DROP TABLE
-- ============================================
IF OBJECT_ID('WRK_TableName') IS NOT NULL
DROP TABLE [WRK_TableName]


-- =============================================
-- CREATE TABLE
-- =============================================

CREATE TABLE [WRK_TableName]
(
	[RowNumber]			INT IDENTITY(1,1)
	,[AAA]				VARCHAR(100)
	,[BBB]				VARCHAR(1000)
	,[DDD]				DATE
	,[EEE]				INT
	,[FFF]				FLOAT
)

-- =============================================
-- TRUNCATE TABLE
-- ============================================
TRUNCATE TABLE [WRK_TableName]

-- =============================================
-- INSERT INTO
-- =============================================
INSERT INTO [WRK_TableName]
(
	[AAA]				
	,[BBB]				
	,[DDD]				
	,[EEE]				
	,[FFF]									
)
SELECT
	[xAAA]				
	,[xBBB]				
	,CONVERT(DATE,[xZZZ],20)				
	,CAST([xWWW] as INT)			
	,CAST([xVVV] as FLOAT)			
FROM [RAW_TableName_YYYYMMDD]
--(x row(s) affected)





END


/*
	SELECT *
	FROM [WRK_TableName]

	SELECT *
	FROM [dbo].[RAW_TableName_YYYYMMDD]
*/
