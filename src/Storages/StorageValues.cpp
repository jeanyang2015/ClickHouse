#include <Storages/IStorage.h>
#include <Storages/ColumnsDescription.h>
#include <Storages/StorageValues.h>
#include <DataStreams/OneBlockInputStream.h>
#include <Processors/Sources/SourceFromSingleChunk.h>
#include <Processors/Pipe.h>


namespace DB
{

StorageValues::StorageValues(const StorageID & table_id_, const ColumnsDescription & columns_, const Block & res_block_)
    : IStorage(table_id_), res_block(res_block_)
{
    setColumns(columns_);
}

Pipes StorageValues::read(
    const Names & column_names,
    const StorageMetadataPtr & metadata_version,
    const SelectQueryInfo & /*query_info*/,
    const Context & /*context*/,
    QueryProcessingStage::Enum /*processed_stage*/,
    size_t /*max_block_size*/,
    unsigned /*num_streams*/)
{
    metadata_version->check(column_names, getVirtuals());

    Pipes pipes;

    Chunk chunk(res_block.getColumns(), res_block.rows());
    pipes.emplace_back(std::make_shared<SourceFromSingleChunk>(res_block.cloneEmpty(), std::move(chunk)));

    return pipes;
}

}
